#ifndef HDR_PATTERN
#define HDR_PATTERN
#include "token.h"

enum PatternType {
	PAT_NONE,
	PAT_IMMNAME,
	PAT_IMMNAME_OFF,
	PAT_NAME,
	PAT_NAME_OFF,
	PAT_P_NAME,
	PAT_P_NAME_OFF,
	PAT_IMM,
	PAT_IMM_OFF,
	PAT_ABS,
	PAT_ABS_OFF,
	PAT_B_IMM,
	PAT_B_IMM_OFF,
	PAT_P_IMM,
	PAT_P_IMM_OFF,
	PAT_P_ABS,
	PAT_P_ABS_OFF,
	PAT_P_B_IMM,
	PAT_P_B_IMM_OFF,
	PAT_I_IMM,
	PAT_A_F,
	PAT_F_A,
	PAT_A_K,
	PAT_K_A,
	PAT_P_B,
	PAT_P_S,
	PAT_B_P,
	PAT_B_S,
	PAT_S_P,
	PAT_S_B,
	PAT_A,
	PAT_F,
	PAT_P,
	PAT_B,
	PAT_EXPORT_SCOPE,
	PAT_SCOPE,
	PAT_ENDSCOPE,
	PAT_EXPORT_LABEL,
	PAT_LABEL,
	PAT_EQUAL,
	PAT_ORIGIN,
	PAT_BYTE,
	PAT_WORD,
	PAT_LONG,
	PAT_QUAD,
	PAT_STRZ,
	PAT_INS,
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

#endif