#include "instruction.h"
#include "parser.h"
#include "token.h"
#include "pattern.h"
#include "symbol.h"
#include "object.h"

extern struct Object *object;
extern struct Segment *segment;
extern struct Symbol *scope;
extern uint32_t curr_address;

static uint32_t normalize(uint32_t address);

uint32_t getOpcode(struct Pattern *pattern) {
	struct Symbol *symbol;
	struct Token *token = pattern->token;
	enum PatternType arg = pattern->type;
	enum TokenType ins = token[0].type;
	bool plus = false;
	if (arg == PAT_INS_NAME || arg == PAT_INS_NAME_OFF) {
		symbol = lookupSymbol(token[1].str, scope);
		if (ins >= TOK_BRZ && ins <= TOK_BRA) {
			arg = PAT_INS_I_IMM;
			if (symbol == NULL) plus = true;
		}
		else {
			if (symbol == NULL) arg = PAT_INS_ABS;
			else switch (symbol->type) {
				case SYM_SCOPE:
				case SYM_LABEL:
				case SYM_NUMBER:      arg = PAT_INS_ABS; break;
				case SYM_B_PLUS_IMM:  arg = PAT_INS_B_IMM; plus = true; break;
				case SYM_B_MINUS_IMM: arg = PAT_INS_B_IMM; break;
				case SYM_P_PLUS_IMM:  arg = PAT_INS_P_IMM; plus = true; break;
			}
		}
	}
	else if (arg == PAT_INS_P_NAME || arg == PAT_INS_P_NAME_OFF) {
		symbol = lookupSymbol(token[3].str, scope);
		if (symbol == NULL) arg = PAT_INS_P_ABS;
		else switch (symbol->type) {
			case SYM_SCOPE:
			case SYM_LABEL:
			case SYM_NUMBER:      arg = PAT_INS_P_ABS; break;
			case SYM_B_PLUS_IMM:  arg = PAT_INS_P_B_IMM; plus = true; break;
			case SYM_B_MINUS_IMM: arg = PAT_INS_P_B_IMM; break;
			case SYM_P_PLUS_IMM:  error("Invalid symbol '%s'.", token[3].str); return 0x100;
		}
	}
	else switch (arg) {
		case PAT_INS_IMM_OFF:
		case PAT_INS_IMMNAME:
		case PAT_INS_IMMNAME_OFF:
			arg = PAT_INS_IMM;
			break;

		case PAT_INS_ABS_OFF:
			arg = PAT_INS_ABS;
			break;
		
		case PAT_INS_B_IMM_OFF:
			arg = PAT_INS_B_IMM;
			break;

		case PAT_INS_B_IMM:
			arg = PAT_INS_B_IMM;
			if (token[2].type == TOK_PLUS) plus = true;
			break;
		
		case PAT_INS_P_IMM_OFF:
			arg = PAT_INS_P_IMM;
			break;
		
		case PAT_INS_P_ABS_OFF:
			arg = PAT_INS_P_ABS;
			break;
		
		case PAT_INS_P_B_IMM_OFF:
			arg = PAT_INS_P_B_IMM;
			break;

		case PAT_INS_P_B_IMM:
			arg = PAT_INS_P_B_IMM;
			if (token[4].type == TOK_PLUS) plus = true;
			break;

		case PAT_INS_I_IMM:
			if (token[4].type == TOK_PLUS) plus = true;
			break;

		default: break;
	}
	static struct {
		uint32_t opcode;
		enum TokenType ins;
		enum PatternType arg;
		bool plus;
	} INS_TBL[] = {
		{0x00, TOK_LOD, PAT_INS_IMM, false},      {0x08, TOK_STW, PAT_INS_ABS, false},
		{0x01, TOK_LOD, PAT_INS_ABS, false},      {0x09, TOK_LDW, PAT_INS_ABS, false},
		{0x02, TOK_LOD, PAT_INS_B_IMM, true},     {0x0A, TOK_LDW, PAT_INS_B_IMM, true},
		{0x03, TOK_LOD, PAT_INS_B_IMM, false},    {0x0B, TOK_LDW, PAT_INS_B_IMM, false},
		{0x04, TOK_LOD, PAT_INS_P_IMM, false},    {0x0C, TOK_LDW, PAT_INS_P_IMM, false},
		{0x05, TOK_LOD, PAT_INS_P_ABS, false},    {0x0D, TOK_LDW, PAT_INS_P_ABS, false},
		{0x06, TOK_LOD, PAT_INS_P_B_IMM, true},   {0x0E, TOK_LDW, PAT_INS_P_B_IMM, true},
		{0x07, TOK_LOD, PAT_INS_P_B_IMM, false},  {0x0F, TOK_LDW, PAT_INS_P_B_IMM, false},

		{0x10, TOK_STW, PAT_INS_B_IMM, true},     {0x18, TOK_STW, PAT_INS_B_IMM, false},
		{0x11, TOK_LEA, PAT_INS_ABS, false},      {0x19, TOK_STO, PAT_INS_ABS, false},
		{0x12, TOK_LEA, PAT_INS_B_IMM, true},     {0x1A, TOK_STO, PAT_INS_B_IMM, true},
		{0x13, TOK_LEA, PAT_INS_B_IMM, false},    {0x1B, TOK_STO, PAT_INS_B_IMM, false},
		{0x14, TOK_LEA, PAT_INS_P_IMM, false},    {0x1C, TOK_STO, PAT_INS_P_IMM, false},
		{0x15, TOK_LEA, PAT_INS_P_ABS, false},    {0x1D, TOK_STO, PAT_INS_P_ABS, false},
		{0x16, TOK_LEA, PAT_INS_P_B_IMM, true},   {0x1E, TOK_STO, PAT_INS_P_B_IMM, true},
		{0x17, TOK_LEA, PAT_INS_P_B_IMM, false},  {0x1F, TOK_STO, PAT_INS_P_B_IMM, false},

		{0x20, TOK_MOV, PAT_INS_A_F, false},      {0x28, TOK_PSH, PAT_INS_A, false},
		{0x21, TOK_MOV, PAT_INS_F_A, false},      {0x29, TOK_PSH, PAT_INS_F, false},
		{0x22, TOK_MOV, PAT_INS_P_B, false},      {0x2A, TOK_PHW, PAT_INS_P, false},
		{0x23, TOK_MOV, PAT_INS_P_S, false},      {0x2B, TOK_PHW, PAT_INS_B, false},
		{0x24, TOK_MOV, PAT_INS_B_P, false},      {0x2C, TOK_PUL, PAT_INS_A, false},
		{0x25, TOK_MOV, PAT_INS_B_S, false},      {0x2D, TOK_PUL, PAT_INS_F, false},
		{0x26, TOK_MOV, PAT_INS_S_P, false},      {0x2E, TOK_PLW, PAT_INS_P, false},
		{0x27, TOK_MOV, PAT_INS_S_B, false},      {0x2F, TOK_PLW, PAT_INS_B, false},

		{0x30, TOK_PSH, PAT_INS_IMM, false},      {0x38, TOK_COP, PAT_INS, false},
		{0x31, TOK_PSH, PAT_INS_ABS, false},      {0x39, TOK_PHW, PAT_INS_ABS, false},
		{0x32, TOK_PSH, PAT_INS_B_IMM, true},     {0x3A, TOK_PHW, PAT_INS_B_IMM, true},
		{0x33, TOK_PSH, PAT_INS_B_IMM, false},    {0x3B, TOK_PHW, PAT_INS_B_IMM, false},
		{0x34, TOK_PSH, PAT_INS_P_IMM, false},    {0x3C, TOK_PHW, PAT_INS_P_IMM, false},
		{0x35, TOK_PSH, PAT_INS_P_ABS, false},    {0x3D, TOK_PHW, PAT_INS_P_ABS, false},
		{0x36, TOK_PSH, PAT_INS_P_B_IMM, true},   {0x3E, TOK_PHW, PAT_INS_P_B_IMM, true},
		{0x37, TOK_PSH, PAT_INS_P_B_IMM, false},  {0x3F, TOK_PHW, PAT_INS_P_B_IMM, false},

		{0x40, TOK_CPW, PAT_INS, false},          // 0x48 is undefined
		{0x41, TOK_PEA, PAT_INS_ABS, false},      {0x49, TOK_PUL, PAT_INS_ABS, false},
		{0x42, TOK_PEA, PAT_INS_B_IMM, true},     {0x4A, TOK_PUL, PAT_INS_B_IMM, true},
		{0x43, TOK_PEA, PAT_INS_B_IMM, false},    {0x4B, TOK_PUL, PAT_INS_B_IMM, false},
		{0x44, TOK_PEA, PAT_INS_P_IMM, false},    {0x4C, TOK_PUL, PAT_INS_P_IMM, false},
		{0x45, TOK_PEA, PAT_INS_P_ABS, false},    {0x4D, TOK_PUL, PAT_INS_P_ABS, false},
		{0x46, TOK_PEA, PAT_INS_P_B_IMM, true},   {0x4E, TOK_PUL, PAT_INS_P_B_IMM, true},
		{0x47, TOK_PEA, PAT_INS_P_B_IMM, false},  {0x4F, TOK_PUL, PAT_INS_P_B_IMM, false},

		{0x50, TOK_NOP, PAT_INS, false},          {0x58, TOK_NOT, PAT_INS_A, false},
		{0x51, TOK_PLW, PAT_INS_ABS, false},      {0x59, TOK_NOT, PAT_INS_ABS, false},
		{0x52, TOK_PLW, PAT_INS_B_IMM, true},     {0x5A, TOK_NOT, PAT_INS_B_IMM, true},
		{0x53, TOK_PLW, PAT_INS_B_IMM, false},    {0x5B, TOK_NOT, PAT_INS_B_IMM, false},
		{0x54, TOK_PLW, PAT_INS_P_IMM, false},    {0x5C, TOK_NOT, PAT_INS_P_IMM, false},
		{0x55, TOK_PLW, PAT_INS_P_ABS, false},    {0x5D, TOK_NOT, PAT_INS_P_ABS, false},
		{0x56, TOK_PLW, PAT_INS_P_B_IMM, true},   {0x5E, TOK_NOT, PAT_INS_P_B_IMM, true},
		{0x57, TOK_PLW, PAT_INS_P_B_IMM, false},  {0x5F, TOK_NOT, PAT_INS_P_B_IMM, false},

		{0x60, TOK_AND, PAT_INS_IMM, false},      {0x68, TOK_ORA, PAT_INS_IMM, false},
		{0x61, TOK_AND, PAT_INS_ABS, false},      {0x69, TOK_ORA, PAT_INS_ABS, false},
		{0x62, TOK_AND, PAT_INS_B_IMM, true},     {0x6A, TOK_ORA, PAT_INS_B_IMM, true},
		{0x63, TOK_AND, PAT_INS_B_IMM, false},    {0x6B, TOK_ORA, PAT_INS_B_IMM, false},
		{0x64, TOK_AND, PAT_INS_P_IMM, false},    {0x6C, TOK_ORA, PAT_INS_P_IMM, false},
		{0x65, TOK_AND, PAT_INS_P_ABS, false},    {0x6D, TOK_ORA, PAT_INS_P_ABS, false},
		{0x66, TOK_AND, PAT_INS_P_B_IMM, true},   {0x6E, TOK_ORA, PAT_INS_P_B_IMM, true},
		{0x67, TOK_AND, PAT_INS_P_B_IMM, false},  {0x6F, TOK_ORA, PAT_INS_P_B_IMM, false},

		{0x70, TOK_ADD, PAT_INS_IMM, false},      {0x78, TOK_ADC, PAT_INS_IMM, false},
		{0x71, TOK_ADD, PAT_INS_ABS, false},      {0x79, TOK_ADC, PAT_INS_ABS, false},
		{0x72, TOK_ADD, PAT_INS_B_IMM, true},     {0x7A, TOK_ADC, PAT_INS_B_IMM, true},
		{0x73, TOK_ADD, PAT_INS_B_IMM, false},    {0x7B, TOK_ADC, PAT_INS_B_IMM, false},
		{0x74, TOK_ADD, PAT_INS_P_IMM, false},    {0x7C, TOK_ADC, PAT_INS_P_IMM, false},
		{0x75, TOK_ADD, PAT_INS_P_ABS, false},    {0x7D, TOK_ADC, PAT_INS_P_ABS, false},
		{0x76, TOK_ADD, PAT_INS_P_B_IMM, true},   {0x7E, TOK_ADC, PAT_INS_P_B_IMM, true},
		{0x77, TOK_ADD, PAT_INS_P_B_IMM, false},  {0x7F, TOK_ADC, PAT_INS_P_B_IMM, false},

		{0x80, TOK_SUB, PAT_INS_IMM, false},      {0x88, TOK_SBC, PAT_INS_IMM, false},
		{0x81, TOK_SUB, PAT_INS_ABS, false},      {0x89, TOK_SBC, PAT_INS_ABS, false},
		{0x82, TOK_SUB, PAT_INS_B_IMM, true},     {0x8A, TOK_SBC, PAT_INS_B_IMM, true},
		{0x83, TOK_SUB, PAT_INS_B_IMM, false},    {0x8B, TOK_SBC, PAT_INS_B_IMM, false},
		{0x84, TOK_SUB, PAT_INS_P_IMM, false},    {0x8C, TOK_SBC, PAT_INS_P_IMM, false},
		{0x85, TOK_SUB, PAT_INS_P_ABS, false},    {0x8D, TOK_SBC, PAT_INS_P_ABS, false},
		{0x86, TOK_SUB, PAT_INS_P_B_IMM, true},   {0x8E, TOK_SBC, PAT_INS_P_B_IMM, true},
		{0x87, TOK_SUB, PAT_INS_P_B_IMM, false},  {0x8F, TOK_SBC, PAT_INS_P_B_IMM, false},

		{0x90, TOK_CMP, PAT_INS_IMM, false},      {0x98, TOK_ASR, PAT_INS_A, false},
		{0x91, TOK_CMP, PAT_INS_ABS, false},      {0x99, TOK_ASR, PAT_INS_ABS, false},
		{0x92, TOK_CMP, PAT_INS_B_IMM, true},     {0x9A, TOK_ASR, PAT_INS_B_IMM, true},
		{0x93, TOK_CMP, PAT_INS_B_IMM, false},    {0x9B, TOK_ASR, PAT_INS_B_IMM, false},
		{0x94, TOK_CMP, PAT_INS_P_IMM, false},    {0x9C, TOK_ASR, PAT_INS_P_IMM, false},
		{0x95, TOK_CMP, PAT_INS_P_ABS, false},    {0x9D, TOK_ASR, PAT_INS_P_ABS, false},
		{0x96, TOK_CMP, PAT_INS_P_B_IMM, true},   {0x9E, TOK_ASR, PAT_INS_P_B_IMM, true},
		{0x97, TOK_CMP, PAT_INS_P_B_IMM, false},  {0x9F, TOK_ASR, PAT_INS_P_B_IMM, false},

		{0xA0, TOK_LSL, PAT_INS_A, false},        {0xA8, TOK_LSR, PAT_INS_A, false},
		{0xA1, TOK_LSL, PAT_INS_ABS, false},      {0xA9, TOK_LSR, PAT_INS_ABS, false},
		{0xA2, TOK_LSL, PAT_INS_B_IMM, true},     {0xAA, TOK_LSR, PAT_INS_B_IMM, true},
		{0xA3, TOK_LSL, PAT_INS_B_IMM, false},    {0xAB, TOK_LSR, PAT_INS_B_IMM, false},
		{0xA4, TOK_LSL, PAT_INS_P_IMM, false},    {0xAC, TOK_LSR, PAT_INS_P_IMM, false},
		{0xA5, TOK_LSL, PAT_INS_P_ABS, false},    {0xAD, TOK_LSR, PAT_INS_P_ABS, false},
		{0xA6, TOK_LSL, PAT_INS_P_B_IMM, true},   {0xAE, TOK_LSR, PAT_INS_P_B_IMM, true},
		{0xA7, TOK_LSL, PAT_INS_P_B_IMM, false},  {0xAF, TOK_LSR, PAT_INS_P_B_IMM, false},

		{0xB0, TOK_ROL, PAT_INS_A, false},        {0xB8, TOK_ROR, PAT_INS_A, false},
		{0xB1, TOK_ROL, PAT_INS_ABS, false},      {0xB9, TOK_ROR, PAT_INS_ABS, false},
		{0xB2, TOK_ROL, PAT_INS_B_IMM, true},     {0xBA, TOK_ROR, PAT_INS_B_IMM, true},
		{0xB3, TOK_ROL, PAT_INS_B_IMM, false},    {0xBB, TOK_ROR, PAT_INS_B_IMM, false},
		{0xB4, TOK_ROL, PAT_INS_P_IMM, false},    {0xBC, TOK_ROR, PAT_INS_P_IMM, false},
		{0xB5, TOK_ROL, PAT_INS_P_ABS, false},    {0xBD, TOK_ROR, PAT_INS_P_ABS, false},
		{0xB6, TOK_ROL, PAT_INS_P_B_IMM, true},   {0xBE, TOK_ROR, PAT_INS_P_B_IMM, true},
		{0xB7, TOK_ROL, PAT_INS_P_B_IMM, false},  {0xBF, TOK_ROR, PAT_INS_P_B_IMM, false},

		{0xC0, TOK_INC, PAT_INS_A, false},        {0xC8, TOK_DEC, PAT_INS_A, false},
		{0xC1, TOK_INC, PAT_INS_ABS, false},      {0xC9, TOK_DEC, PAT_INS_ABS, false},
		{0xC2, TOK_INC, PAT_INS_B_IMM, true},     {0xCA, TOK_DEC, PAT_INS_B_IMM, true},
		{0xC3, TOK_INC, PAT_INS_B_IMM, false},    {0xCB, TOK_DEC, PAT_INS_B_IMM, false},
		{0xC4, TOK_INC, PAT_INS_P_IMM, false},    {0xCC, TOK_DEC, PAT_INS_P_IMM, false},
		{0xC5, TOK_INC, PAT_INS_P_ABS, false},    {0xCD, TOK_DEC, PAT_INS_P_ABS, false},
		{0xC6, TOK_INC, PAT_INS_P_B_IMM, true},   {0xCE, TOK_DEC, PAT_INS_P_B_IMM, true},
		{0xC7, TOK_INC, PAT_INS_P_B_IMM, false},  {0xCF, TOK_DEC, PAT_INS_P_B_IMM, false},

		{0xD0, TOK_ICT, PAT_INS_A, false},        {0xD8, TOK_INC, PAT_INS_P, false},
		{0xD1, TOK_ICT, PAT_INS_ABS, false},      {0xD9, TOK_DEC, PAT_INS_P, false},
		{0xD2, TOK_ICT, PAT_INS_B_IMM, true},     {0xDA, TOK_ICT, PAT_INS_P, false},
		{0xD3, TOK_ICT, PAT_INS_B_IMM, false},    {0xDB, TOK_DCT, PAT_INS_P, false},
		{0xD4, TOK_DCT, PAT_INS_A, false},        {0xDC, TOK_SET, PAT_INS_IMM, false},
		{0xD5, TOK_DCT, PAT_INS_ABS, false},      {0xDD, TOK_CLR, PAT_INS_IMM, false},
		{0xD6, TOK_DCT, PAT_INS_B_IMM, true},     {0xDE, TOK_MOV, PAT_INS_A_K, false},
		{0xD7, TOK_DCT, PAT_INS_B_IMM, false},    {0xDF, TOK_MOV, PAT_INS_K_A, false},

		{0xE0, TOK_BRZ, PAT_INS_I_IMM, true},     {0xE8, TOK_BRN, PAT_INS_I_IMM, true},
		{0xE1, TOK_BRZ, PAT_INS_I_IMM, false},    {0xE9, TOK_BRN, PAT_INS_I_IMM, false},
		{0xE2, TOK_BNZ, PAT_INS_I_IMM, true},     {0xEA, TOK_BNN, PAT_INS_I_IMM, true},
		{0xE3, TOK_BNZ, PAT_INS_I_IMM, false},    {0xEB, TOK_BNN, PAT_INS_I_IMM, false},
		{0xE4, TOK_BRC, PAT_INS_I_IMM, true},     {0xEC, TOK_BRG, PAT_INS_I_IMM, true},
		{0xE5, TOK_BRC, PAT_INS_I_IMM, false},    {0xED, TOK_BRG, PAT_INS_I_IMM, false},
		{0xE6, TOK_BNC, PAT_INS_I_IMM, true},     {0xEE, TOK_BNG, PAT_INS_I_IMM, true},
		{0xE7, TOK_BNC, PAT_INS_I_IMM, false},    {0xEF, TOK_BNG, PAT_INS_I_IMM, false},

		{0xF0, TOK_BRA, PAT_INS_I_IMM, true},     {0xF8, TOK_JSR, PAT_INS_ABS, false},
		{0xF1, TOK_BRA, PAT_INS_I_IMM, false},    {0xF9, TOK_JSL, PAT_INS_ABS, false},
		{0xF2, TOK_VEC, PAT_INS_P_B_IMM, true},   {0xFA, TOK_RTS, PAT_INS, false},
		{0xF3, TOK_VEC, PAT_INS_P_B_IMM, false},  {0xFB, TOK_RTS, PAT_INS_IMM, false},
		{0xF4, TOK_JMP, PAT_INS_ABS, false},      {0xFC, TOK_RTL, PAT_INS, false},
		{0xF5, TOK_JPL, PAT_INS_ABS, false},      {0xFD, TOK_RTL, PAT_INS_IMM, false},
		{0xF6, TOK_ENT, PAT_INS_IMM, false},      {0xFE, TOK_RTI, PAT_INS, false},
		{0xF7, TOK_EXT, PAT_INS_IMM, false},      {0xFF, TOK_BRK, PAT_INS, false},
	};
	size_t INS_TBL_LEN = sizeof INS_TBL / sizeof INS_TBL[0];
	for (size_t i = 0; i < INS_TBL_LEN; ++i) {
		if (
			ins == INS_TBL[i].ins &&
			arg == INS_TBL[i].arg &&
			plus == INS_TBL[i].plus
		) return INS_TBL[i].opcode;
	}
	return 0x100;
}

void addArgData(struct Pattern *pattern) {
	struct Symbol *symbol;
	uint32_t data, offset;
	struct Token *token = pattern->token;
	bool is_long = token[0].type == TOK_JPL || token[0].type == TOK_JSL;
	if (token[0].type >= TOK_BRZ && token[0].type <= TOK_BRA && pattern->type == PAT_INS_NAME) {
		symbol = lookupSymbol(token[1].str, scope);
		if (symbol == NULL) {
			addReference(segment, curr_address, dupStr(token[1].str), 0, 1);
			++curr_address;
		}
		else switch (symbol->type) {
			case SYM_SCOPE:
			case SYM_LABEL:
				data = normalize(curr_address - symbol->value - 1);
				if (data > 255) error("Branch to '%s' is too far.", token[1].str);
				else {
					setBranchBackwards(segment);
					addData(segment, data, 1);
					++curr_address;
				}
				break;

			default: error("Invalid symbol '%s'.", token[1].str);
		}
	}
	else switch (pattern->type) {
		case PAT_INS_IMMNAME:
			symbol = lookupSymbol(token[2].str, scope);
			if (symbol == NULL) {
				addReference(segment, curr_address, dupStr(token[2].str), 0, 1);
				++curr_address;
			}
			else if (symbol->type == SYM_NUMBER) {
				addData(segment, symbol->value, 1);
				++curr_address;
			}
			else error("Invalid symbol '%s'.", token[2].str);
			break;

		case PAT_INS_IMMNAME_OFF:
			offset = parseNumber(&token[5]);
			symbol = lookupSymbol(token[2].str, scope);
			if (symbol == NULL) {
				addReference(segment, curr_address, dupStr(token[2].str), offset, 1);
				++curr_address;
			}
			else if (symbol->type == SYM_NUMBER) {
				addData(segment, symbol->value + offset, 1);
				++curr_address;
			}
			else error("Invalid symbol '%s'.", token[2].str);
			break;

		case PAT_INS_NAME:
			symbol = lookupSymbol(token[1].str, scope);
			if (symbol == NULL) {
				addReference(segment, curr_address, dupStr(token[1].str), 0, is_long ? 3 : 2);
				curr_address += is_long ? 3 : 2;
			}
			else switch (symbol->type) {
				case SYM_SCOPE:
				case SYM_LABEL:
					data = is_long ? symbol->value : normalize(symbol->value);
					addData(segment, data, is_long ? 3 : 2);
					curr_address += is_long ? 3 : 2;
					break;

				case SYM_NUMBER:
					addData(segment, symbol->value, is_long ? 3 : 2);
					curr_address += is_long ? 3 : 2;
					break;

				case SYM_B_PLUS_IMM:
				case SYM_P_PLUS_IMM:
					addData(segment, symbol->value, 1);
					++curr_address;
					break;

				case SYM_B_MINUS_IMM:
					addData(segment, symbol->value - 1, 1);
					++curr_address;
					break;
			}
			break;

		case PAT_INS_NAME_OFF:
			offset = parseNumber(&token[4]);
			symbol = lookupSymbol(token[1].str, scope);
			if (symbol == NULL) {
				addReference(segment, curr_address, dupStr(token[1].str), offset, is_long ? 3 : 2);
				curr_address += is_long ? 3 : 2;
			}
			else switch (symbol->type) {
				case SYM_SCOPE:
				case SYM_LABEL:
					data = is_long ? symbol->value : normalize(symbol->value);
					addData(segment, data + offset, is_long ? 3 : 2);
					curr_address += is_long ? 3 : 2;
					break;

				case SYM_NUMBER:
					addData(segment, symbol->value + offset, is_long ? 3 : 2);
					curr_address += is_long ? 3 : 2;
					break;

				case SYM_B_PLUS_IMM:
				case SYM_P_PLUS_IMM:
					addData(segment, symbol->value + offset, 1);
					++curr_address;
					break;

				case SYM_B_MINUS_IMM:
					addData(segment, symbol->value - 1 - offset, 1);
					++curr_address;
					break;
			}
			break;

		case PAT_INS_P_NAME:
			symbol = lookupSymbol(token[3].str, scope);
			if (symbol == NULL) {
				addReference(segment, curr_address, dupStr(token[3].str), 0, 2);
				curr_address += 2;
			}
			else switch (symbol->type) {
				case SYM_SCOPE:
				case SYM_LABEL:
					data = is_long ? symbol->value : normalize(symbol->value);
					addData(segment, data, 2);
					curr_address += 2;
					break;

				case SYM_NUMBER:
					addData(segment, symbol->value, 2);
					curr_address += 2;
					break;

				case SYM_B_PLUS_IMM:
				case SYM_P_PLUS_IMM:
					addData(segment, symbol->value, 1);
					++curr_address;
					break;

				case SYM_B_MINUS_IMM:
					addData(segment, symbol->value - 1, 1);
					++curr_address;
					break;
			}
			break;

		case PAT_INS_P_NAME_OFF:
			offset = parseNumber(&token[6]);
			symbol = lookupSymbol(token[3].str, scope);
			if (symbol == NULL) {
				addReference(segment, curr_address, dupStr(token[3].str), offset, 2);
				curr_address += 2;
			}
			else switch (symbol->type) {
				case SYM_SCOPE:
				case SYM_LABEL:
					data = is_long ? symbol->value : normalize(symbol->value);
					addData(segment, data + offset, 2);
					curr_address += 2;
					break;

				case SYM_NUMBER:
					addData(segment, symbol->value + offset, 2);
					curr_address += 2;
					break;

				case SYM_B_PLUS_IMM:
				case SYM_P_PLUS_IMM:
					addData(segment, symbol->value + offset, 1);
					++curr_address;
					break;

				case SYM_B_MINUS_IMM:
					addData(segment, symbol->value - 1 - offset, 1);
					++curr_address;
					break;
			}
			break;

		case PAT_INS_IMM:
			data = parseNumber(&token[2]);
			addData(segment, data, 1);
			++curr_address;
			break;

		case PAT_INS_IMM_OFF:
			data = parseNumber(&token[2]);
			offset = parseNumber(&token[5]);
			addData(segment, data + offset, 1);
			++curr_address;
			break;

		case PAT_INS_ABS:
			data = parseNumber(&token[1]);
			addData(segment, data, is_long ? 3 : 2);
			curr_address += is_long ? 3 : 2;
			break;

		case PAT_INS_ABS_OFF:
			data = parseNumber(&token[1]);
			offset = parseNumber(&token[4]);
			addData(segment, data + offset, 2);
			curr_address += 2;
			break;

		case PAT_INS_I_IMM:
		case PAT_INS_P_IMM:
		case PAT_INS_B_IMM:
			data = parseNumber(&token[4]);
			if (token[2].type == TOK_MINUS) --data;
			addData(segment, data, 1);
			++curr_address;
			break;

		case PAT_INS_P_IMM_OFF:
		case PAT_INS_B_IMM_OFF:
			data = parseNumber(&token[4]);
			offset = parseNumber(&token[7]);
			if (token[2].type == TOK_MINUS) data = data - 1 - offset;
			else data += offset;
			addData(segment, data, 1);
			++curr_address;
			break;

		case PAT_INS_P_ABS:
			data = parseNumber(&token[3]);
			addData(segment, data, 2);
			curr_address += 2;
			break;

		case PAT_INS_P_ABS_OFF:
			data = parseNumber(&token[3]);
			offset = parseNumber(&token[6]);
			addData(segment, data + offset, 2);
			curr_address += 2;
			break;

		case PAT_INS_P_B_IMM:
			data = parseNumber(&token[6]);
			addData(segment, data, 1);
			++curr_address;
			break;

		case PAT_INS_P_B_IMM_OFF:
			data = parseNumber(&token[6]);
			offset = parseNumber(&token[9]);
			if (token[4].type == TOK_MINUS) data = data - 1 - offset;
			else data += offset;
			addData(segment, data, 1);
			++curr_address;
			break;

		default: break;
	}
}

uint32_t normalize(uint32_t address) {
	return 0;
}