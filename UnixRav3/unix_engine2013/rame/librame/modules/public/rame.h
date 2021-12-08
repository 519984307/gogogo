#ifndef _PUBINC_H_
#define _PUBINC_H_

#include <locale.h>
#include "../../include/pubinc.h"
#include "../../include/2010/interface.h"
#include "../publib/filepath.h"
#include "../publib/stdcom.h"
#include "../publib/rxez.h"
#include "../publib/propimpl.hpp"
#include "../publib/xpropimpl.h"
#include "../publib/unionname.h"
#include "../publib/stdcom10.h"
#include "rstype.h"
#include "dllexport.h"
#include "../../../include/interlocked.h"
#include "../../include/sdk.define.h"

/*#include "../publib/fileutils.h"*/


//////////////////////////////////////////////////////////////////////////
struct SetLocale
{
	SetLocale() { setlocale(LC_ALL,""); }
};
static const SetLocale _GLOBAL_SET_LOCALE_;

#endif	//_PUBINC_H_
