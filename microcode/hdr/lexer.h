#ifndef HDR_LEXER
#define HDR_LEXER

enum TokenType {
	TOK_RST,
	TOK_INT,
	TOK_OPCODE,
	TOK_NAME,
	TOK_NEWLINE,
	TOK_SYMBOL,
	TOK_FLAG,
	TOK_LBRACE,
	TOK_RBRACE,
};

enum Flag {
	FLG_Z = 0x1,
	FLG_N = 0x2,
	FLG_C = 0x4,
	FLG_I = 0x8,
};

struct Token {
	enum TokenType type;
	uint32_t line, col;
	union {
		uint8_t opcode;
		char symbol[3];
		char *name;
		enum Flag flag;
	};
};

struct Token *buildTokens(
	size_t *token_len_ptr
);

void freeTokenArr(
	struct Token *token,
	size_t token_len
);

#endif