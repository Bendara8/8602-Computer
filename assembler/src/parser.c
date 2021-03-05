#include "parser.h"
#include "instruction.h"
#include "token.h"
#include "pattern.h"
#include "symbol.h"
#include "object.h"
#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>

struct Object *object = NULL;
struct Segment *segment = NULL;
struct Symbol *scope = NULL;
uint32_t curr_address = 0;

static void parseExportScope(void);
static void parseScope(void);
static void parseEndscope(void);
static void parseExportLabel(void);
static void parseLabel(void);
static void parseDefine(void);
static void parseOrigin(void);
static void parseData(size_t size);
static void parseStrz(void);
static void parseIns(void);
static uint32_t parseDecimal(char *str);
static uint32_t parseHex(char *str);
static uint32_t parseBinary(char *str);
static uint32_t parseName(char *str, size_t size);

static char *path = NULL;
static uint32_t err_ct = 0;
static struct Pattern *curr_pattern = NULL;
static bool no_origin = true;

void parsePatternsIntoObject(char *path_) {
	path = path_;
	object = newObject(path);
	resetSymbolList();
	while (peekPattern() != NULL) {
		curr_pattern = nextPattern();
		switch (curr_pattern->type) {
			case PAT_EXPORT_SCOPE: parseExportScope(); break;
			case PAT_SCOPE:        parseScope(); break;
			case PAT_ENDSCOPE:     parseEndscope(); break;
			case PAT_EXPORT_LABEL: parseExportLabel(); break;
			case PAT_LABEL:        parseLabel(); break;
			case PAT_DEFINE:       parseDefine(); break;
			case PAT_ORIGIN:       parseOrigin(); break;
			case PAT_BYTE:         parseData(1); break;
			case PAT_WORD:         parseData(2); break;
			case PAT_LONG:         parseData(3); break;
			case PAT_QUAD:         parseData(4); break;
			case PAT_STRZ:         parseStrz(); break;
			default:
				if ((curr_pattern->token[0].type & TOK_GROUP) == TOK_INS) parseIns(); 
				else error("Invalid pattern.");
		}
	}
}

void parseExportScope(void) {
	if (no_origin) error("Cannot declare scope without an origin.");
	else if (scope != NULL) error("Cannot nest scopes.");
	else {
		addExportSymbol(object, SYM_SCOPE, NULL, curr_pattern->token[1].str, curr_address);
		scope = addSymbol(SYM_SCOPE, NULL, curr_pattern->token[1].str, curr_address);
	}
}

void parseScope(void) {
	if (no_origin) error("Cannot declare scope without an origin.");
	else if (scope != NULL) error("Cannot nest scopes.");
	else {
		scope = addSymbol(SYM_SCOPE, NULL, curr_pattern->token[0].str, curr_address);
	}
}

void parseEndscope(void) {
	if (scope == NULL) error("Left scope without entering one.");
	else scope = NULL;
}

void parseExportLabel(void) {
	if (no_origin) error("Cannot declare label without an origin.");
	else if (scope != NULL) error("Cannot export label from within a scope.");
	else {
		addExportSymbol(object, SYM_LABEL, scope, curr_pattern->token[1].str, curr_address);
		addSymbol(SYM_LABEL, scope, curr_pattern->token[1].str, curr_address);
	}
}

void parseLabel(void) {
	if (no_origin) error("Cannot declare label without origin.");
	else addSymbol(SYM_LABEL, scope, curr_pattern->token[0].str, curr_address);
}

void parseDefine(void) {
	if (peekPattern() == NULL) error("Empty define '%s'.", curr_pattern->token[0].str);
	else {
		struct Pattern *next_pattern = nextPattern();
		enum SymbolType symbol_type;
		uint32_t symbol_value;
		switch (next_pattern->type) {
			case PAT_NUMBER:
				symbol_type = SYM_NUMBER;
				symbol_value = parseNumber(&next_pattern->token[0]);
				break;

			case PAT_B_PLUS_IMM:
				symbol_type = SYM_B_PLUS_IMM;
				symbol_value = parseNumber(&next_pattern->token[3]);
				break;

			case PAT_B_MINUS_IMM:
				symbol_type = SYM_B_MINUS_IMM;
				symbol_value = parseNumber(&next_pattern->token[3]);
				break;

			case PAT_P_PLUS_IMM:
				symbol_type = SYM_P_PLUS_IMM;
				symbol_value = parseNumber(&next_pattern->token[3]);
				break;

			default: error("Invalid define."); return;
		}
		addSymbol(symbol_type, scope, curr_pattern->token[0].str, symbol_value);
	}
}

void parseOrigin(void) {
	curr_address = parseHex(curr_pattern->token[1].str);
	segment = newSegment(object, curr_address);
	no_origin = false;
}

void parseData(size_t size) {
	struct Token *token = &curr_pattern->token[1];
	for (size_t i = 0; token[i].type != TOK_NEWLINE; ++i) {
		uint32_t data;
		switch (token[i].type) {
			case TOK_DECIMAL: data = parseDecimal(token[i].str); break;
			case TOK_HEX:     data = parseHex(token[i].str); break;
			case TOK_BINARY:  data = parseBinary(token[i].str); break;
			case TOK_NAME:    data = parseName(token[i].str, size); break;
			default: error("'%s' is not a number or a name.", token[i].str); return;
		}
		addData(segment, data, size);
		curr_address += size;
	}
}

void parseStrz(void) {
	char *str = curr_pattern->token[1].str;
	for (size_t i = 0; str[i] != '\0'; ++i) {
		if (str[i] != '\\') {
			addData(segment, str[i], 1);
			++curr_address;
		}
		else switch (str[++i]) {
			case '0': addData(segment, '\0', 1); ++curr_address; break;
			case 'a': addData(segment, '\a', 1); ++curr_address; break;
			case 'b': addData(segment, '\b', 1); ++curr_address; break;
			case 't': addData(segment, '\t', 1); ++curr_address; break;
			case 'n': addData(segment, '\n', 1); ++curr_address; break;
			case 'v': addData(segment, '\v', 1); ++curr_address; break;
			case 'f': addData(segment, '\f', 1); ++curr_address; break;
			case 'r': addData(segment, '\r', 1); ++curr_address; break;
			case 'e': addData(segment, '\x1B', 1); ++curr_address; break;
			case '"': addData(segment, '"', 1); ++curr_address; break;
			case '\\': addData(segment, '\\', 1); ++curr_address; break;
			case 'x':
				if (isxdigit(str[i + 1]) && isxdigit(str[i + 2])) {
					char data = isdigit(str[i + 1]) ? str[i + 1] & 0xF : toupper(str[i + 1]) - 'A' + 0xA;
					data <<= 4;
					data |= isdigit(str[i + 2]) ? str[i + 2] & 0xF : toupper(str[i + 2]) - 'A' + 0xA;
					addData(segment, data, 1);
					++curr_address;
					i += 2;
				}
				else {
					error("'\\x' does not have a valid hex value.");
					return;
				}
				break;

			default: error("'%c' is an invalid escape character.", str[i]); return;
		}
	}
	addData(segment, '\0', 1);
	++curr_address;
}

void parseIns(void) {
	uint32_t opcode = getOpcode(curr_pattern);
	if (opcode <= 0xFF) {
		addData(segment, opcode, 1);
		++curr_address;
		addArgData(curr_pattern);
	}
	else error("Invalid argument for instruction '%s'.", curr_pattern->token[0].str);
}

// change input to a token pointer
uint32_t parseNumber(struct Token *token) {
	switch (token->type) {
		case TOK_DECIMAL: return parseDecimal(token->str);
		case TOK_HEX:     return parseHex(token->str);
		case TOK_BINARY:  return parseBinary(token->str);
		default: return 0;
	}
}

uint32_t parseDecimal(char *str) {
	return 0;
}

uint32_t parseHex(char *str) {
	return 0;
}

uint32_t parseBinary(char *str) {
	return 0;
}

uint32_t parseName(char *str, size_t size) {
	struct Symbol *symbol = lookupSymbol(str, scope);
	if (symbol == NULL) {
		addReference(segment, curr_address, str, 0, size);
		curr_address += size;
		return 0;
	}
	else switch (symbol->type) {
		case SYM_SCOPE:
		case SYM_LABEL:
		case SYM_NUMBER:
			return symbol->value;

		default: error("'%s' is not a scope, label, or number.", str); return 0;
	}
}

void error(char *format, ...) {
	va_list args;
	printf("(%s:%u) ", path, curr_pattern->token[0].line);
	vprintf(format, args);
	printf("\n");
	++err_ct;
}