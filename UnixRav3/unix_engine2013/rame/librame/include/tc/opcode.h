#ifndef __ti_opcode__
#define __ti_opcode__

#define DEFINE_OPCODE
#include "opdef.h"

enum 
{
	#include "optable.h"

	OPCODE_MAX,
};

#define CODE_END		0xFF

#if OPCODE_MAX > (CODE_END-1) 
#error "Too many opcodes\n"
#endif

#undef	DEFINE_OPCODE

#define PKS_MIN_LEN		5



#endif