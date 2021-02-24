#ifndef HDR_TOKEN
#define HDR_TOKEN

enum TokenType {
	TOK_NONE,
	// seperators
	TOK_NEWLINE,
	TOK_COMMA,
	TOK_DOT,
	TOK_LBRACE,
	TOK_RBRACE,
	// operators
	TOK_HASH,
	TOK_PLUS,
	TOK_MINUS,
	// registers
	TOK_A,
	TOK_F,
	TOK_K,
	TOK_P,
	TOK_B,
	TOK_S,
	TOK_I,
	// directives
	TOK_ORIGIN,
	TOK_EXPORT,
	TOK_BYTE,
	TOK_WORD,
	TOK_LONG,
	TOK_QUAD,
	TOK_STRZ,
	TOK_EQUALS,
	// instructions
	TOK_LOD, TOK_LDW, TOK_LEA,
	TOK_STO, TOK_STW,
	TOK_MOV,
	TOK_PSH, TOK_PHW, TOK_PEA,
	TOK_PUL, TOK_PLW,
	TOK_COP, TOK_CPW,
	TOK_NOP,
	TOK_NOT, TOK_AND, TOK_ORA,
	TOK_ADD, TOK_ADC,
	TOK_SUB, TOK_SBC,
	TOK_CMP,
	TOK_ASR,
	TOK_LSL, TOK_LSR,
	TOK_ROL, TOK_ROR,
	TOK_INC, TOK_DEC,
	TOK_ICT, TOK_DCT,
	TOK_SET, TOK_CLR,
	TOK_BRZ, TOK_BNZ,
	TOK_BRC, TOK_BNC,
	TOK_BRN, TOK_BNN,
	TOK_BRG, TOK_BNG,
	TOK_BRA,
	TOK_VEC,
	TOK_JMP, TOK_JPL,
	TOK_ENT, TOK_EXT,
	TOK_JSR, TOK_JSL,
	TOK_RTS, TOK_RTL, TOK_RTI,
	TOK_BRK,
	// literals
	TOK_DECIMAL,
	TOK_HEX,
	TOK_BINARY,
	TOK_STRING,
	TOK_CHAR,
	// name
	TOK_NAME,
};

struct Token {
	enum TokenType type;
	uint32_t line;
	char *str;
};

void initTokenQueue(
	void
);

void deinitTokenQueue(
	void
);

void resetTokenQueue(
	void
);

struct Token *newToken(
	void
);

enum TokenType searchTokenCharTable(
	char chr
);

enum TokenType searchTokenStringTable(
	char *str
);

#endif