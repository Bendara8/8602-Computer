#include <stdlib.h>
#include <stdio.h> // TEMP
#include "circuit.h"
#include "chip.h"
#include "net.h"
#include "lexer.h"
#include "token.h"
#include "error.h"

static unsigned long elapsed_time = 0;

static void parseBusDef(struct TokenQue *tok_que, struct BusVec *bus_vec, struct NetVec *net_vec) {
	assertNextTokenType(tok_que, TOK_BEG_BRACE);
	while (peekTokenType(tok_que) != TOK_END_BRACE) {
		assertNextTokenType(tok_que, TOK_SYMBOL);
		char *name = currToken(tok_que)->str;
		assertNextTokenType(tok_que, TOK_NUM);
		unsigned width = currToken(tok_que)->num;
		if (width == 0) raiseError(ERROR_INVALID_NUM, (int)width);
		else addBus(bus_vec, net_vec, name, width);
	}
	nextToken(tok_que);
}

static void parseNetDef(struct TokenQue *tok_que, struct NetVec *net_vec) {
	assertNextTokenType(tok_que, TOK_BEG_BRACE);
	while (peekTokenType(tok_que) != TOK_END_BRACE) {
		assertNextTokenType(tok_que, TOK_SYMBOL);
		char *name = currToken(tok_que)->str;
		struct Net *net = addNet(net_vec, name);
		if (peekTokenType(tok_que) == TOK_EQUALS) {
			nextToken(tok_que);
			assertNextTokenType(tok_que, TOK_NUM);
			int num = (int)currToken(tok_que)->num;
			if (num != 0 && num != 1) {
				setErrorLine(currToken(tok_que)->line);
				raiseError(ERROR_INVALID_NUM, num);
				raiseAbort(ABORT_FATAL_ERROR);
			}
			net->val = num;
		}
	}
	nextToken(tok_que);
}

static size_t buildIdxArr(size_t **idx_vec_ptr, size_t max_len, struct TokenQue *tok_que) {
	size_t idx_vec_len = 0, idx_vec_cap = 32;
	*idx_vec_ptr = malloc(sizeof (size_t) * idx_vec_cap);
	if (!*idx_vec_ptr) raiseAbort(ABORT_ALLOCATION);
	addFreeTarget(*idx_vec_ptr);
	while (peekTokenType(tok_que) == TOK_NUM) {
		if (idx_vec_len == idx_vec_cap) {
			setErrorLine(peekToken(tok_que)->line);
			raiseError(ERROR_MANY_NETS, idx_vec_cap);
			raiseAbort(ABORT_FATAL_ERROR);
		}
		size_t num = nextToken(tok_que)->num;
		if (num >= max_len) {
			setErrorLine(currToken(tok_que)->line);
			raiseError(ERROR_INVALID_NUM, num);
			raiseAbort(ABORT_FATAL_ERROR);
		}
		(*idx_vec_ptr)[idx_vec_len++] = num;
		if (peekTokenType(tok_que) == TOK_COMMA) nextToken(tok_que);
		else if (peekTokenType(tok_que) == TOK_DASH) {
			nextToken(tok_que);
			assertNextTokenType(tok_que, TOK_NUM);
			size_t beg_num = (*idx_vec_ptr)[idx_vec_len - 1];
			size_t end_num = currToken(tok_que)->num;
			if (end_num <= beg_num) {
				setErrorLine(currToken(tok_que)->line);
				raiseError(ERROR_INVALID_NUM, end_num);
				raiseAbort(ABORT_FATAL_ERROR);
			}
			for (size_t idx = beg_num + 1; idx <= end_num; ++idx) {
				if (idx_vec_len == idx_vec_cap) {
					setErrorLine(currToken(tok_que)->line);
					raiseError(ERROR_MANY_NETS, idx_vec_cap);
					raiseAbort(ABORT_FATAL_ERROR);
				}
				(*idx_vec_ptr)[idx_vec_len++] = idx;
			}
			if (peekTokenType(tok_que) == TOK_COMMA) nextToken(tok_que);
		}
		else break;
	}
	return idx_vec_len;
}

static void assignAllDstWithNet(
	struct Net ***dst_ptr_arr,
	size_t dst_ptr_arr_len,
	struct Net *net
) {
	for (size_t i = 0; i < dst_ptr_arr_len; ++i) {
		*dst_ptr_arr[i] = net;
	}
}

static void assignDstWithNetIdxArr(
	struct Net ***dst_ptr_arr,
	size_t dst_ptr_arr_len,
	size_t *idx_arr,
	struct Bus *bus,
	struct NetVec *net_vec
) {
	for (size_t i = 0; i < dst_ptr_arr_len; ++i) {
		*dst_ptr_arr[i] = &net_vec->buf[bus->net_idx + idx_arr[i]];
	}
}

static void assignNets(
	struct Net **dst_arr,
	size_t dst_arr_len,
	struct TokenQue *tok_que,
	struct BusVec *bus_vec,
	struct NetVec *net_vec
) {
	struct Net ***dst_ptr_arr;
	size_t dst_ptr_arr_len = 0;
	size_t *idx_arr;
	size_t idx_arr_len;
	nextToken(tok_que);
	assertNextTokenType(tok_que, TOK_COLON);
	dst_ptr_arr_len = buildIdxArr(&idx_arr, dst_arr_len, tok_que);
	dst_ptr_arr = malloc(sizeof (struct Net **) * dst_ptr_arr_len);
	addFreeTarget(dst_ptr_arr);
	for (size_t i = 0; i < dst_ptr_arr_len; ++i) {
		dst_ptr_arr[i] = &dst_arr[idx_arr[i]];
	}
	assertNextTokenType(tok_que, TOK_EQUALS);
	if (peekTokenType(tok_que) == TOK_NULL) {
		nextToken(tok_que);
		assignAllDstWithNet(dst_ptr_arr, dst_ptr_arr_len, NULL);
		return;
	}
	assertNextTokenType(tok_que, TOK_SYMBOL);
	char *name = currToken(tok_que)->str;
	if (peekTokenType(tok_que) == TOK_COLON) {
		nextToken(tok_que);
		struct Bus *bus = findBus(bus_vec, name);
		if (!bus) {
			setErrorLine(currToken(tok_que)->line);
			raiseError(ERROR_NOT_BUS, name);
			raiseAbort(ABORT_FATAL_ERROR);
		}
		idx_arr_len = buildIdxArr(&idx_arr, bus->width, tok_que);
		if (idx_arr_len == 1) {
			assignAllDstWithNet(
				dst_ptr_arr,
				dst_ptr_arr_len,
				&net_vec->buf[bus->net_idx + idx_arr[0]]
			);
		}
		else {
			if (idx_arr_len != dst_ptr_arr_len) {
				setErrorLine(currToken(tok_que)->line);
				raiseError(ERROR_NUM_NETS);
				raiseAbort(ABORT_FATAL_ERROR);
			}
			assignDstWithNetIdxArr(
				dst_ptr_arr,
				dst_ptr_arr_len,
				idx_arr,
				bus,
				net_vec
			);
		}
	}
	else {
		struct Net *net = findNet(net_vec, name);
		if (!net) {
			setErrorLine(currToken(tok_que)->line);
			raiseError(ERROR_NOT_NET, name);
			raiseAbort(ABORT_FATAL_ERROR);
		}
		assignAllDstWithNet(dst_ptr_arr, dst_ptr_arr_len, net);
	}
}

static void parseChipDef(
	struct TokenQue *tok_que,
	struct ChipVec *chip_vec,
	struct BusVec *bus_vec,
	struct NetVec *net_vec,
	struct Circuit *circ
) {
	assertNextTokenType(tok_que, TOK_BEG_BRACE);
	while (peekTokenType(tok_que) != TOK_END_BRACE) {
		assertNextTokenType(tok_que, TOK_SYMBOL);
		char *type_str = currToken(tok_que)->str;
		char *name = (peekTokenType(tok_que) == TOK_SYMBOL) ? nextToken(tok_que)->str : NULL;
		struct Chip *chip = addChip(chip_vec, type_str, name, circ);
		assertNextTokenType(tok_que, TOK_BEG_BRACE);
		while (peekTokenType(tok_que) != TOK_END_BRACE) {
			switch (peekTokenType(tok_que)) {
				case TOK_IN:
					assignNets(chip->in, CHIP_DATA[chip->type].in, tok_que, bus_vec, net_vec);
					break;
				case TOK_OUT:
					assignNets(chip->out, CHIP_DATA[chip->type].out, tok_que, bus_vec, net_vec);
					break;
				case TOK_NONE:
					setErrorLine(currToken(tok_que)->line);
					raiseError(ERROR_UNEXPECTED_EOF);
					raiseAbort(ABORT_FATAL_ERROR);
					break;
				default:
					setErrorLine(peekToken(tok_que)->line);
					raiseError(ERROR_UNEXPECTED_TOKEN);
					raiseAbort(ABORT_FATAL_ERROR);
					break;
			}
		}
		nextToken(tok_que);
	}
	nextToken(tok_que);
}

void initCircuit(struct Circuit *circ, char *path) {
	struct TokenVec tok_vec;
	struct TokenQue tok_que;
	initTokenVec(&tok_vec, 512);
	initTokenQue(&tok_que, &tok_vec);
	lexFile(path, &tok_vec);
	initBusVec(&circ->bus_vec, 64);
	initNetVec(&circ->net_vec, 256);
	initChipVec(&circ->chip_vec, 64);
	if (getErrorFlag()) raiseAbort(ABORT_FATAL_ERROR);
	while (currToken(&tok_que)) {
		switch (currToken(&tok_que)->type) {
			case TOK_BUS:
				parseBusDef(&tok_que, &circ->bus_vec, &circ->net_vec);
				break;
			case TOK_NET:
				parseNetDef(&tok_que, &circ->net_vec);
				break;
			case TOK_CHIP:
				parseChipDef(&tok_que, &circ->chip_vec, &circ->bus_vec, &circ->net_vec, circ);
				break;
			case TOK_NONE:
				setErrorLine(currToken(&tok_que)->line);
				raiseError(ERROR_UNEXPECTED_EOF);
				raiseAbort(ABORT_FATAL_ERROR);
				break;
			default:
				setErrorLine(currToken(&tok_que)->line);
				raiseError(ERROR_UNEXPECTED_TOKEN);
				raiseAbort(ABORT_FATAL_ERROR);
				break;
		}
		nextToken(&tok_que);
	}
	circ->update_list.head = NULL;
	circ->empty_list.head = allocNetUpdateBlock(512);
}

void stepCircuit(struct Circuit *circ) {
	// step chips and add net updates
	for (size_t i = 0; i < circ->chip_vec.len; ++i) {
		stepChip(&circ->chip_vec.buf[i]);
	}

	// clear changed status
	for (size_t i = 0; i < circ->net_vec.len; ++i) {
		circ->net_vec.buf[i].changed = 0;
	}

	// step and apply net updates
	struct NetUpdate *temp = circ->update_list.head, **last = &circ->update_list.head;
	while (temp) {
		last = stepNetUpdate(temp, last, &circ->empty_list.head);
		temp = *last;
	}

	++elapsed_time;
}

unsigned long getElapsedTime(void) {
	return elapsed_time;
}

void addNetUpdate(struct Circuit *circ, struct Net *target, int val, int delay) {
	if (val != 0 && val != 1) {
		printf("Adding out of bounds net...\n");
	}
	if (!target) return;
	struct NetUpdate *temp = circ->update_list.head, **last = &circ->update_list.head;
	while (temp) {
		if (temp->target == target && temp->delay >= delay) {
			*last = temp->next;
			temp->next = circ->empty_list.head;
			circ->empty_list.head = temp;
			temp->target = NULL;
			temp->val = 0;
			temp->delay = 0;
		}
		else last = &temp->next;
		temp = *last;
	}
	if (!circ->empty_list.head) circ->empty_list.head = allocNetUpdateBlock(512);
	temp = circ->empty_list.head->next;
	circ->empty_list.head->next = circ->update_list.head;
	circ->update_list.head = circ->empty_list.head;
	circ->empty_list.head = temp;
	circ->update_list.head->target = target;
	circ->update_list.head->val = val;
	circ->update_list.head->delay = delay;
}

size_t numNetsUpdated(struct Circuit *circ) {
	size_t count = 0;
	for (size_t i = 0; i < circ->net_vec.len; ++i) {
		if (circ->net_vec.buf[i].changed) ++count;
	}
	return count;
}

size_t lenSchedule(struct Circuit *circ) {
	struct NetUpdate *curr = circ->update_list.head;
	size_t len = 0;
	while (curr) {
		curr = curr->next;
		++len;
	}
	return len;
}