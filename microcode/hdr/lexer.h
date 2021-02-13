#ifndef HDR_LEXER
#define HDR_LEXER

enum TokenType {
	TOK_OPCODE,
	TOK_NAME,
	TOK_SYMBOL,
	TOK_FLAG,
	TOK_STAR,
	TOK_TILDE,
};

enum Flag {
	FLG_Z = 0x1,
	FLG_N = 0x2,
	FLG_C = 0x4,
	FLG_I = 0x8,
	FLG_ALL = 0xFF,
};

struct Token {
	enum TokenType type;
	uint32_t line, col;
	union {
		uint8_t opcode;
		char symbol[2];
		char *name;
		struct {
			enum Flag flag;
			uint8_t value;
		};
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