#ifndef __FN_DEFINED__
#define __FN_DEFINED__

	
	
	

#endif

#undef op_r
#undef op_rr_rv	
#undef op_rr	
#undef op_rt	
#undef op_tr	
#undef op_rv	
#undef op_j_b_w	
#undef op_rj
#undef op_rtrr	
#undef op_		
#undef op_rvis	
#undef op_rviis	
#undef op_rvs
#undef op_tr_tv
#undef op_r_i
#undef op_i
#undef op_rvipks

#if defined(DEFINE_OPCODE)

#define op_r(x)			op_##x##_r,
#define op_rr_rv(x)		op_##x##_rr, op_##x##_rv,
#define op_rr(x)		op_##x##_rr,
#define op_rt(x)		op_##x##_rt,
#define op_tr(x)		op_##x##_tr,
#define op_rv(x)		op_##x##_rv,
#define op_rtrr(x)		op_##x##_rtrr,
#define op_(x)			op_##x,
#define op_rvis(x)		op_##x##_rvis,
#define op_rviis(x)		op_##x##_rviis,
#define op_rj(x)		op_##x##_rj,
#define op_j_b_w(x)		op_##x##_jb, op_##x##_jw,
#define op_rvs(x)		op_##x##_rvs,
#define op_tr_tv(x)		op_##x##_tr, op_##x##_tv, 
#define op_r_i(x)		op_##x##_r, op_##x##_i, 
#define op_i(x)			op_##x##_i, 
#define op_rvipks(x)	op_##x##_rvipks,

#elif defined(DEFINE_ASSEMBLE_HANDLER) 

#define op_r(x)			{ #x, assemble_r<op_##x##_r> },
#define op_rr_rv(x)		{ #x, assemble_rr_or_rv<op_##x##_rr,op_##x##_rv> },
#define op_rr(x)		{ #x, assemble_rr<op_##x##_rr> },
#define op_rt(x)		{ #x, assemble_rt<op_##x##_rt> },
#define op_tr(x)		{ #x, assemble_tr<op_##x##_tr> },
#define op_rv(x)		{ #x, assemble_rv<op_##x##_rv> },
#define op_rtrr(x)		{ #x, assemble_rtrr<op_##x##_rtrr> },
#define op_(x)			{ #x, assemble_onebyte<op_##x> },
#define op_rvis(x)		{ #x, assemble_rvis<op_##x##_rvis> },
#define op_rviis(x)		{ #x, assemble_rviis<op_##x##_rviis> },
#define op_rj(x)		{ #x, assemble_rj<op_##x##_rj> },
#define op_j_b_w(x)		{ #x, assemble_j<op_##x##_jb, op_##x##_jw> },
#define op_rvs(x)		{ #x, assemble_rvs<op_##x##_rvs> },
#define op_tr_tv(x)		{ #x, assemble_tr_or_tv<op_##x##_tr, op_##x##_tv> },
#define op_r_i(x)		{ #x, assemble_r_or_i<op_##x##_r, op_##x##_i> },
#define op_i(x)			{ #x, assemble_i<op_##x##_i> },
#define op_rvipks(x)	{ #x, assemble_rvipks<op_##x##_rvipks> },


#elif defined(DECLARE_EXECUTE_HANDLER)
#define opHandler(x)	TCER FASTCALL _##x( binrstm& bins, EMUX& tcx )
#define op_r(x)			opHandler(x##_r);
#define op_rr_rv(x)		opHandler(x##_rr); opHandler(x##_rv); 
#define op_rr(x)		opHandler(x##_rr);
#define op_rt(x)		opHandler(x##_rt);
#define op_tr(x)		opHandler(x##_tr);
#define op_rv(x)		opHandler(x##_rv); 
#define op_rtrr(x)		opHandler(x##_rtrr); 
#define op_(x)			opHandler(x); 
#define op_rvis(x)		opHandler(x##_rvis); 
#define op_rviis(x)		opHandler(x##_rviis); 
#define op_rj(x)		opHandler(x##_rj); 
#define op_j_b_w(x)		opHandler(x##_jb); opHandler(x##_jw); 
#define op_rvs(x)		opHandler(x##_rvs); 
#define op_tr_tv(x)		opHandler(x##_tr); opHandler(x##_tv); 
#define op_r_i(x)		opHandler(x##_r); opHandler(x##_i);
#define op_i(x)			opHandler(x##_i); 
#define op_rvipks(x)	opHandler(x##_rvipks); 

#elif defined(DEFINE_EXECUTE_TABLE) || defined(DEFINE_OPFN_TABLE)

#define op_r(x)			&(_##x##_r),
#define op_rr_rv(x)		&(_##x##_rr), &(_##x##_rv),
#define op_rr(x)		&(_##x##_rr),
#define op_rt(x)		&(_##x##_rt),
#define op_tr(x)		&(_##x##_tr),
#define op_rv(x)		&(_##x##_rv),
#define op_rtrr(x)		&(_##x##_rtrr),
#define op_(x)			&(_##x),
#define op_rvis(x)		&(_##x##_rvis),
#define op_rviis(x)		&(_##x##_rviis),
#define op_rj(x)		&(_##x##_rj),
#define op_j_b_w(x)		&(_##x##_jb), &(_##x##_jw),
#define op_rvs(x)		&(_##x##_rvs),
#define op_tr_tv(x)		&(_##x##_tr), &(_##x##_tv),
#define op_r_i(x)		&(_##x##_r), &(_##x##_i),
#define op_i(x)			&(_##x##_i),
#define op_rvipks(x)	&(_##x##_rvipks),

#elif defined(DEFINE_DISASM_HANDLER)

#define op_r(x)			disasm_r<op_##x##_r>,
#define op_rr_rv(x)		disasm_rr<op_##x##_rr>, disasm_rv<op_##x##_rv>,
#define op_rr(x)		disasm_rr<op_##x##_rr>,
#define op_rt(x)		disasm_rt<op_##x##_rt>,
#define op_tr(x)		disasm_tr<op_##x##_tr>,
#define op_rv(x)		disasm_rv<op_##x##_rv>,
#define op_rtrr(x)		disasm_rtrr<op_##x##_rtrr>,
#define op_(x)			disasm_onebyte<op_##x>,
#define op_rvis(x)		disasm_rvis<op_##x##_rvis>,
#define op_rviis(x)		disasm_rviis<op_##x##_rviis>,
#define op_rj(x)		disasm_rj<op_##x##_rj>,
#define op_j_b_w(x)		disasm_jb<op_##x##_jb>, diasm_jw<op_##x##_jw>,
#define op_rvs(x)		disasm_rvs<op_##x##_rvs>,
#define op_tr_tv(x)		disasm_tr<op_##x##_tr>, disasm_tv<op_##x##_tv>,
#define op_r_i(x)		disasm_tr<op_##x##_r>, disasm_tv<op_##x##_i>,
#define op_i(x)			disasm_tv<op_##x##_i>,
#define op_rvipks(x)	disasm_rvipks<op_##x##_rvipks>,

#elif defined(DEFINE_INSNAME_TABLE)

#define op_r(x)			#x,
#define op_rr_rv(x)		#x, #x,
#define op_rr(x)		#x,
#define op_rt(x)		#x,
#define op_tr(x)		#x,
#define op_rv(x)		#x,
#define op_rtrr(x)		#x,
#define op_(x)			#x,
#define op_rvis(x)		#x,
#define op_rviis(x)		#x,
#define op_rj(x)		#x,
#define op_j_b_w(x)		#x, #x,
#define op_rvs(x)		#x,
#define op_tr_tv(x)		#x, #x,
#define op_r_i(x)		#x, #x,
#define op_i(x)			#x,
#define op_rvipks(x)	#x,

#elif defined(DECLARE_NATIVE_HANDLER)
#define nativeHandler(x)	int _##x( binrstm& bins, TextBlock& text )
#define op_r(x)				nativeHandler(x##_r);
#define op_rr_rv(x)			nativeHandler(x##_rr); nativeHandler(x##_rv); 
#define op_rr(x)			nativeHandler(x##_rr);
#define op_rt(x)			nativeHandler(x##_rt);
#define op_tr(x)			nativeHandler(x##_tr);
#define op_rv(x)			nativeHandler(x##_rv); 
#define op_rtrr(x)			nativeHandler(x##_rtrr); 
#define op_(x)				nativeHandler(x); 
#define op_rvis(x)			nativeHandler(x##_rvis); 
#define op_rviis(x)			nativeHandler(x##_rviis); 
#define op_rj(x)			nativeHandler(x##_rj); 
#define op_j_b_w(x)			nativeHandler(x##_jb); nativeHandler(x##_jw); 
#define op_rvs(x)			nativeHandler(x##_rvs); 
#define op_tr_tv(x)			nativeHandler(x##_tr); nativeHandler(x##_tv); 
#define op_r_i(x)			nativeHandler(x##_r); nativeHandler(x##_i);
#define op_i(x)				nativeHandler(x##_i); 
#define op_rvipks(x)		nativeHandler(x##_rvipks); 

#elif defined(DECLARE_JIT_HANDLER)
#define nativeHandler(x)	TCER _##x( binrstm& bins, JitContext& jit )
#define op_r(x)				jitHandler(x##_r);
#define op_rr_rv(x)			jitHandler(x##_rr); jitHandler(x##_rv); 
#define op_rr(x)			jitHandler(x##_rr);
#define op_rt(x)			jitHandler(x##_rt);
#define op_tr(x)			jitHandler(x##_tr);
#define op_rv(x)			jitHandler(x##_rv); 
#define op_rtrr(x)			jitHandler(x##_rtrr); 
#define op_(x)				jitHandler(x); 
#define op_rvis(x)			jitHandler(x##_rvis); 
#define op_rviis(x)			jitHandler(x##_rviis); 
#define op_rj(x)			jitHandler(x##_rj); 
#define op_j_b_w(x)			jitHandler(x##_jb); jitHandler(x##_jw); 
#define op_rvs(x)			jitHandler(x##_rvs); 
#define op_tr_tv(x)			jitHandler(x##_tr); jitHandler(x##_tv); 
#define op_r_i(x)			jitHandler(x##_r); jitHandler(x##_i);
#define op_i(x)				jitHandler(x##_i); 
#define op_rvipks(x)		jitHandler(x##_rvipks); 

#endif