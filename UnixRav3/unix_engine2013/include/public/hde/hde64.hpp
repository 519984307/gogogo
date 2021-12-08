/*
 * Hacker Disassembler Engine 64 C
 * Copyright (c) 2008-2009, Vyacheslav Patkov.
 * All rights reserved.
 *
 */

#ifndef __RX_COM_HDE64_H_H_H__
#define __RX_COM_HDE64_H_H_H__

#include "hde64.h"
#include "table64.h"

#ifdef __cplusplus
extern "C" {
#endif

STATICINLINE unsigned int hde64_disasm(bptr_t& code, hde64s *hs)
{
    uint8_t x, c, cflags, opcode, pref = 0;
    uint8_t *ht = hde64_table, m_mod, m_reg, m_rm, disp_size = 0;
    uint8_t op64 = 0;

	bptr_t & p = code;

    memset(hs,0,sizeof(hde64s));
	hs->x64 = 1;
    for (x = 16; x; x--)
	{
		if( !p.next(c) ) return 0;
        switch(c) 
		{
            case 0xf3:
                hs->p_rep = c;
                pref |= PRE_F3;
                break;
            case 0xf2:
                hs->p_rep = c;
                pref |= PRE_F2;
                break;
            case 0xf0:
                hs->p_lock = c;
                pref |= PRE_LOCK;
                break;
            case 0x26: case 0x2e: case 0x36:
            case 0x3e: case 0x64: case 0x65:
                hs->p_seg = c;
                pref |= PRE_SEG;
                break;
            case 0x66:
                hs->p_66 = c;
                pref |= PRE_66;
                break;
            case 0x67:
                hs->p_67 = c;
                pref |= PRE_67;
                break;
            default:
                goto pref_done;
        }
	}
  pref_done:

    hs->flags = (uint32_t)pref << 23;

    if (!pref)
        pref |= PRE_NONE;

    if ((c & 0xf0) == 0x40) 
	{
        hs->flags |= F64_PREFIX_REX;

		if ((hs->rex_w = (c & 0xf) >> 3) )
		{
			uint8_t _c = 0;
			if( !p.curr(_c) ) return 0;
			if( (_c & 0xf8) == 0xb8 )
				op64++;
		}
        //if ((hs->rex_w = (c & 0xf) >> 3) && (*p & 0xf8) == 0xb8)
        //    op64++;
		hs->rex_r = (c & 7) >> 2;
        hs->rex_x = (c & 3) >> 1;
        hs->rex_b = c & 1;

		if( !p.next(c) ) return 0;
        if ((c & 0xf0) == 0x40) 
		{
            opcode = c;
            goto error_opcode;
        }
    }

    if ((hs->opcode = c) == 0x0f) 
	{
		if( !p.next(c) ) return 0;
        hs->opcode2 = c;
        ht += DELTA_OPCODES;
    } 
	else if (c >= 0xa0 && c <= 0xa3) 
	{
        op64 ++;
        if (pref & PRE_67)
            pref |= PRE_66;
        else
            pref &= ~PRE_66;
    }

    opcode = c;
    cflags = ht[ht[opcode / 4] + (opcode % 4)];

    if (cflags == C_ERROR) {
      error_opcode:
        hs->flags |= F64_ERROR | F64_ERROR_OPCODE;
        cflags = 0;
        if ((opcode & -3) == 0x24)
            cflags++;
    }

    x = 0;
    if (cflags & C_GROUP) {
        uint16_t t;
        t = *(uint16_t *)(ht + (cflags & 0x7f));
        cflags = (uint8_t)t;
        x = (uint8_t)(t >> 8);
    }

    if (hs->opcode2) {
        ht = hde64_table + DELTA_PREFIXES;
        if (ht[ht[opcode / 4] + (opcode % 4)] & pref)
            hs->flags |= F64_ERROR | F64_ERROR_OPCODE;
    }

    if (cflags & C_MODRM) 
	{
		if( !p.next(c) ) return 0;
        hs->flags |= F64_MODRM;
        hs->modrm = c;
        hs->modrm_mod = m_mod = c >> 6;
        hs->modrm_rm = m_rm = c & 7;
        hs->modrm_reg = m_reg = (c & 0x3f) >> 3;

        if (x && ((x << m_reg) & 0x80))
            hs->flags |= F64_ERROR | F64_ERROR_OPCODE;

        if (!hs->opcode2 && opcode >= 0xd9 && opcode <= 0xdf) {
            uint8_t t = opcode - 0xd9;
            if (m_mod == 3) {
                ht = hde64_table + DELTA_FPU_MODRM + t*8;
                t = ht[m_reg] << m_rm;
            } else {
                ht = hde64_table + DELTA_FPU_REG;
                t = ht[t] << m_reg;
            }
            if (t & 0x80)
                hs->flags |= F64_ERROR | F64_ERROR_OPCODE;
        }

        if (pref & PRE_LOCK) {
            if (m_mod == 3) {
                hs->flags |= F64_ERROR | F64_ERROR_LOCK;
            } else {
                uint8_t *table_end, op = opcode;
                if (hs->opcode2) {
                    ht = hde64_table + DELTA_OP2_LOCK_OK;
                    table_end = ht + DELTA_OP_ONLY_MEM - DELTA_OP2_LOCK_OK;
                } else {
                    ht = hde64_table + DELTA_OP_LOCK_OK;
                    table_end = ht + DELTA_OP2_LOCK_OK - DELTA_OP_LOCK_OK;
                    op &= -2;
                }
                for (; ht != table_end; ht++)
                    if (*ht++ == op) {
                        if (!((*ht << m_reg) & 0x80))
                            goto no_lock_error;
                        else
                            break;
                    }
                hs->flags |= F64_ERROR | F64_ERROR_LOCK;
              no_lock_error:
                ;
            }
        }

        if (hs->opcode2) 
		{
            switch (opcode) 
			{
                case 0x20: case 0x22:
                    m_mod = 3;
                    if (m_reg > 4 || m_reg == 1)
                        goto error_operand;
                    else
                        goto no_error_operand;
                case 0x21: case 0x23:
                    m_mod = 3;
                    if (m_reg == 4 || m_reg == 5)
                        goto error_operand;
                    else
                        goto no_error_operand;
            }
        } 
		else 
		{
            switch (opcode) 
			{
                case 0x8c:
                    if (m_reg > 5)
                        goto error_operand;
                    else
                        goto no_error_operand;
                case 0x8e:
                    if (m_reg == 1 || m_reg > 5)
                        goto error_operand;
                    else
                        goto no_error_operand;
            }
        }

        if (m_mod == 3) 
		{
            uint8_t *table_end;
            if (hs->opcode2)
			{
                ht = hde64_table + DELTA_OP2_ONLY_MEM;
                table_end = ht + sizeof(hde64_table) - DELTA_OP2_ONLY_MEM;
            } 
			else 
			{
                ht = hde64_table + DELTA_OP_ONLY_MEM;
                table_end = ht + DELTA_OP2_ONLY_MEM - DELTA_OP_ONLY_MEM;
            }
            for (; ht != table_end; ht += 2)
			{
                if (*ht++ == opcode) 
				{
                    if (*ht++ & pref && !((*ht << m_reg) & 0x80))
					{
                        goto error_operand;
					}
                    else
					{
                        break;
					}
                }
			}
            goto no_error_operand;
        } else if (hs->opcode2) {
            switch (opcode) {
                case 0x50: case 0xd7: case 0xf7:
                    if (pref & (PRE_NONE | PRE_66))
                        goto error_operand;
                    break;
                case 0xd6:
                    if (pref & (PRE_F2 | PRE_F3))
                        goto error_operand;
                    break;
                case 0xc5:
                    goto error_operand;
            }
            goto no_error_operand;
        } else
            goto no_error_operand;

	error_operand:
		hs->flags |= F64_ERROR | F64_ERROR_OPERAND;
	no_error_operand:

		if( !p.next(c) ) return 0;
        //c = *p++;
        if (m_reg <= 1) 
		{
            if (opcode == 0xf6)
			{
                cflags |= C_IMM8;
			}
            else if (opcode == 0xf7)
			{
                cflags |= C_IMM_P66;
			}
        }

        switch (m_mod) 
		{
            case 0:
                if (pref & PRE_67) {
                    if (m_rm == 6)
                        disp_size = 2;
                } else
                    if (m_rm == 5)
                        disp_size = 4;
                break;
            case 1:
                disp_size = 1;
                break;
            case 2:
                disp_size = 2;
                if (!(pref & PRE_67))
                    disp_size <<= 1;
        }

        if (m_mod != 3 && m_rm == 4)
		{
            hs->flags |= F64_SIB;
			if( !p.seek(1) ) return 0;
            hs->sib = c;
            hs->sib_scale = c >> 6;
            hs->sib_index = (c & 0x3f) >> 3;
            if ((hs->sib_base = c & 7) == 5 && !(m_mod & 1))
                disp_size = 4;
        }

        if( !p.seek(-1) ) return 0;
        switch (disp_size) 
		{
            case 1:
                hs->flags |= F64_DISP8;
				if( !p.fetch(hs->disp.disp8) ) 
					return 0;
                break;
            case 2:
                hs->flags |= F64_DISP16;
				if( !p.fetch(hs->disp.disp16) ) 
					return 0;
                break;
            case 4:
                hs->flags |= F64_DISP32;
				if( !p.fetch(hs->disp.disp32) ) 
					return 0;
        }
    } else if (pref & PRE_LOCK)
        hs->flags |= F64_ERROR | F64_ERROR_LOCK;

	hs->len2 = p.offset();

    if (cflags & C_IMM_P66) {
        if (cflags & C_REL32) {
            if (pref & PRE_66) {
                hs->flags |= F64_IMM16 | F64_RELATIVE;
				if( !p.fetch(hs->imm.imm16) ) 
					return 0;
                goto disasm_done;
            }
            goto rel32_ok;
        }
        if (op64) {
            hs->flags |= F64_IMM64;
			if( !p.fetch(hs->imm.imm64) ) 
				return 0;
        } else if (!(pref & PRE_66)) {
            hs->flags |= F64_IMM32;
			if( !p.fetch(hs->imm.imm32) ) 
				return 0;
        } else
            goto imm16_ok;
    }


    if (cflags & C_IMM16) {
      imm16_ok:
        hs->flags |= F64_IMM16;
		if( !p.fetch(hs->imm.imm16) ) 
			return 0;
    }
    if (cflags & C_IMM8) {
        hs->flags |= F64_IMM8;
		if( !p.fetch(hs->imm.imm8) ) 
			return 0;
    }

    if (cflags & C_REL32) {
      rel32_ok:
        hs->flags |= F64_IMM32 | F64_RELATIVE;
		if( !p.fetch(hs->imm.imm32) ) 
			return 0;
    } else if (cflags & C_REL8) {
        hs->flags |= F64_IMM8 | F64_RELATIVE;
		if( !p.fetch(hs->imm.imm8) ) 
			return 0;
    }

  disasm_done:

    if ((hs->len = p.offset() ) > 15) {
        hs->flags |= F64_ERROR | F64_ERROR_LENGTH;
        hs->len = 15;
    }

    return (unsigned int)hs->len;
}

#ifdef __cplusplus
}
#endif

#endif //__RX_COM_HDE64_H_H_H__
