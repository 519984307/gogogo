#ifndef __JDIFF_EXPORT__
#define __JDIFF_EXPORT__

#define JDIFF_LARGEFILE

namespace JojoDiff 
{
	/**
	 * Output routine constants
	 */
	enum PATCH_CMD
	{
		ESC  =  0xA7,    /* Escape       */
		MOD  =  0xA6,    /* Modify       */
		INS  =  0xA5,    /* Insert       */
		DEL  =  0xA4,    /* Delete       */
		EQL  =  0xA3,    /* Equal        */
		BKT  =  0xA2,    /* Backtrace    */
	};
	
#ifdef JDIFF_LARGEFILE

	#if !defined(_MSC_VER)
		typedef long long	offset_t;
		#define PRIzd		"lld"
	#else
		typedef __int64		offset_t;
		#define PRIzd		"I64d"
	#endif

#else

	typedef long offset_t;
	#define PRIzd	"ld"

#endif


	/* JDiff perform "addressed" file accesses when reading,
	 * hence this abstract wrapper class to translate between streamed and addressed access.
	 *
	 * This abstraction also allows anyone to easily apply JDiff to his own data structures, by providing
	 * a JFile descendant to JDiff.
	 */
	class JFileIn 
	{
	public:
		/**
		 * Get byte at specified address. Auto-increments the address to the next byte.
		 * Soft read ahead will return an EOB when reading out of the buffer.
		 *
		 * @param azPos		position to read from, gets incremented by one after reading
		 * @param aiTyp		0=read, 1=hard read ahead, 2=soft read ahead
		 * @return 			the read character or EOF or EOB.
		 */
		virtual int get (const offset_t &azPos, const int aiTyp) = 0 ;

		/**
		 * Return number of seek operations performed.
		 */
		virtual long seekcount() = 0;
	};

	struct JFileBOut
	{
		virtual bool putch( int uch ) = 0;
	};

	enum EXICODE
	{
		J_DIFF		= 1,
		J_SAME		= 0,
		//////////////////////////////////////////////////////////////////////////
		IO_EOF		= -1,
		IO_EOB		= IO_EOF-1,
		//////////////////////////////////////////////////////////////////////////
		ERR_SEEK	= -10,
		ERR_IO		= -11,
		ERR_OTHER	= -12,
	};

	// return EXICODE
	int Diff( JFileIn& apFilOrg, JFileIn& apFilNew, JFileBOut& apOut, 
		const int aiHshSze = 32*1024*1024, 
		const int abSrcBkt = true, 
		const int aiSrcScn = true, 
		const int aiMchMax = 128, 
		const int aiMchMin = 16, 
		const int aiAhdMax = 4096*1024, 
		const bool abCmpAll = true );

	//int Patch( JFileIn& apFilOrg, JFileIn& aPatch, JFileOut& apOut, 

}; /* namespace */

#endif