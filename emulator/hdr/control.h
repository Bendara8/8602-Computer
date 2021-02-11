#ifndef HDR_CONTROL
#define HDR_CONTROL

enum Control {
	CTRL_AO = 0x000000,
	CTRL_FO = 0x000001,
	CTRL_KO = 0x000002,
	CTRL_MO = 0x000003,
	CTRL_PL = 0x000004,
	CTRL_PH = 0x00000C,
	CTRL_BL = 0x000005,
	CTRL_BH = 0x00000D,
	CTRL_IL = 0x000006,
	CTRL_IH = 0x00000E,
	CTRL_HO = 0x000007,
	CTRL_OD = 0x00000F,

	CTRL_AI = 0x000000,
	CTRL_XI = 0x000010,
	CTRL_KI = 0x000020,
	CTRL_MI = 0x000030,
	CTRL_CI = 0x000040,
	CTRL_TL = 0x000050,
	CTRL_TH = 0x000060,
	CTRL_ND = 0x000070,

	CTRL_FI = 0x000080, // inverted
	
	CTRL_PO = 0x000000,
	CTRL_BO = 0x000100,
	CTRL_SO = 0x000200,
	CTRL_IO = 0x000300,
	CTRL_TO = 0x000400, // inverted
	
	CTRL_PI = 0x000000,
	CTRL_BI = 0x000800,
	CTRL_SI = 0x001000,
	CTRL_II = 0x001800,
	CTRL_NA = 0x002000, // inverted
	
	CTRL_FD = 0x004000,
	CTRL_KZ = 0x008000, // inverted
	CTRL_XZ = 0x010000, // inverted
	CTRL_XN = 0x020000,
	CTRL_XC = 0x040000,
	CTRL_EZ = 0x080000, // inverted
	CTRL_EN = 0x100000,
	CTRL_EC = 0x200000,
	
	CTRL_ES = 0x000000,
	CTRL_ER = 0x400000,
	CTRL_EA = 0x800000,
	CTRL_EO = 0xC00000,
};

enum Control readControl(
	uint8_t step,
	uint8_t flags,
	uint8_t opcode,
	bool interrupt_0,
	bool interrupt_1,
	bool reset
);

#endif