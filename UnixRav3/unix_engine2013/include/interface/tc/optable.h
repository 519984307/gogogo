	op_rr_rv(add)	/*0		1*/
	op_rr_rv(sub)	/*2		3*/
	op_rr_rv(mul)	/*4		5*/
	op_rr_rv(div)	/*6		7*/
	op_rv(rem)		/*8		*/
	op_r(neg)		/*9		*/
	op_r(inc)		/*A		*/
	op_r(dec)		/*B		*/

	op_rr_rv(and)	/*C		D*/
	op_rr_rv(or)	/*E		F*/
	op_rr_rv(xor)	/*10	11*/
	op_r(not)		/*12	*/

	op_rr_rv(shl)	/*13	14*/
	op_rr_rv(shr)	/*15	16*/
	op_rr_rv(rol)	/*17	18*/
	op_rr_rv(ror)	/*19	1A*/

	op_rr_rv(mov)	/*1B	1C*/
	op_rr(lodb)		/*1D*/
	op_rr(lodbu)	/*1E*/
	op_rr(lodw)		/*1F*/
	op_rr(lodwu)	/*20*/
	op_rr(lod)		/*21*/

	op_rr_rv(crc)	/*22	23*/

	op_rt(settype)	/*24	25*/
	op_rt(cvttype)	/*26	27*/

	op_rr_rv(cmpu)	/*28	29*/
	op_rr_rv(cmp)	/*2A	2B*/

	op_tr(jmpr)		/*2C*/
	op_j_b_w(jmp)	/*2D	2E*/
	op_j_b_w(jlt)
	op_j_b_w(jle)
	op_j_b_w(jeq)
	op_j_b_w(jne)
	op_j_b_w(jge)
	op_j_b_w(jgt)

	op_rj(loop)

	op_rvis(scmp)
	op_rvis(sicmp)
	op_rvis(swcmp)
	op_rvis(swicmp)
	op_rviis(sfind)
	op_rviis(sifind)
	op_rviis(swfind)
	op_rviis(swifind)

	op_rtrr(inrange)

	op_(done)
	op_(donelt)
	op_(donele)
	op_(doneeq)
	op_(donene)
	op_(donege)
	op_(donegt)
	op_(exit)
	op_(exitlt)
	op_(exitle)
	op_(exiteq)
	op_(exitne)
	op_(exitge)
	op_(exitgt)

	op_tr_tv(setarg)
	op_rr_rv(invoke)
	op_rvs(ivokex)

	op_rr_rv(btst)

	op_(bp)
	op_r(dump)

	op_rr_rv(invoke2)

	op_rr_rv(test)

	/* in kill / sto to comm_buffer */
	/* sto value, dest */

	op_rr(stob)
	op_rr(stobu)
	op_rr(stow)
	op_rr(stowu)
	op_rr(sto)

	op_rv(probe)

	op_r(slen)		/*strlen*/
	op_r(wslen)		/*strlen*/

	op_r(update)		/* update a reg */
	op_(updatea)		/* update all reg */

	op_rvipks(pkscmp)
	op_rvipks(pksicmp)

	op_rr(abs)

	op_rvis(numin)		// 
	op_rtrr(numin2)		// 
