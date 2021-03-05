#ifndef HDR_PATTERN
#define HDR_PATTERN
#include "token.h"

enum PatternType {
	PAT_NONE,
	PAT_INS_IMMNAME,
	PAT_INS_IMMNAME_OFF,
	PAT_INS_NAME,
	PAT_INS_NAME_OFF,
	PAT_INS_P_NAME,
	PAT_INS_P_NAME_OFF,
	PAT_INS_IMM,
	PAT_INS_IMM_OFF,
	PAT_INS_ABS,
	PAT_INS_ABS_OFF,
	PAT_INS_B_IMM,
	PAT_INS_B_IMM_OFF,
	PAT_INS_P_IMM,
	PAT_INS_P_IMM_OFF,
	PAT_INS_P_ABS,
	PAT_INS_P_ABS_OFF,
	PAT_INS_P_B_IMM,
	PAT_INS_P_B_IMM_OFF,
	PAT_INS_I_IMM,
	PAT_INS_A_F,
	PAT_INS_F_A,
	PAT_INS_A_K,
	PAT_INS_K_A,
	PAT_INS_P_B,
	PAT_INS_P_S,
	PAT_INS_B_P,
	PAT_INS_B_S,
	PAT_INS_S_P,
	PAT_INS_S_B,
	PAT_INS_A,
	PAT_INS_F,
	PAT_INS_P,
	PAT_INS_B,
	PAT_INS,
	PAT_EXPORT_SCOPE,
	PAT_SCOPE,
	PAT_ENDSCOPE,
	PAT_EXPORT_LABEL,
	PAT_LABEL,
	PAT_DEFINE,
	PAT_ORIGIN,
	PAT_BYTE,
	PAT_WORD,
	PAT_LONG,
	PAT_QUAD,
	PAT_STRZ,
	PAT_NUMBER,
	PAT_B_PLUS_IMM,
	PAT_B_MINUS_IMM,
	PAT_P_PLUS_IMM,
};

struct Pattern {
	enum PatternType type;
	struct Token *token;
};

void initPatternQueue(
	void
);

void deinitPatternQueue(
	void
);

void resetPatternQueue(
	void
);

struct Pattern *newPattern(
	void
);

struct Pattern *nextPattern(
	void
);

struct Pattern *peekPattern(
	void
);

#endif