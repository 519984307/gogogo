#ifndef __HDE_X64__
#define __HDE_X64__

#pragma pack(push,1)

typedef struct {
	uint8_t x64;
	uint8_t len;
	uint8_t	len2;
	uint8_t p_rep;
	uint8_t p_lock;
	uint8_t p_seg;
	uint8_t p_66;
	uint8_t p_67;
	uint8_t rex;
	uint8_t rex_w;
	uint8_t rex_r;
	uint8_t rex_x;
	uint8_t rex_b; 
	uint8_t opcode;
	uint8_t opcode2;
	uint8_t modrm;
	uint8_t modrm_mod;
	uint8_t modrm_reg;
	uint8_t modrm_rm;
	uint8_t sib;
	uint8_t sib_scale;
	uint8_t sib_index;
	uint8_t sib_base;
	union
	{
#ifdef RS_BIG_ENDIAN
		struct{
			uint8_t nouse1[7];
			uint8_t imm8;
		};
		struct{
			uint16_t nouse2[3];
			uint16_t imm16;
		};
		struct{
			uint32_t nouse3;
			uint32_t imm32;
		};
		uint64_t imm64;
#else
		uint8_t imm8;
		uint16_t imm16;
		uint32_t imm32;
		uint64_t imm64;
#endif
	} imm;
	union
	{
#ifdef RS_BIG_ENDIAN
		struct{
			uint8_t nouse1[3];
			uint8_t disp8;
		};
		struct{
			uint16_t nouse;
			uint16_t disp16;
		};
		uint32_t disp32;
#else
		uint8_t disp8;
		uint16_t disp16;
		uint32_t disp32;
#endif
	} disp;
	uint32_t flags;
} hde64s;

#pragma pack(pop)

#endif