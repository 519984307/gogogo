#ifndef __LIBLOADER_UTIL_H__
#define __LIBLOADER_UTIL_H__







//----------------------------------------------------------------------------------
//for libloader sp
/*#define TABLETYPE_INDEX				0x100
#define TABLETYPE_TABLE				0x000
#define TABLETYPE_DUMMY				0x800
#define IsIndex(w)					((w & TABLETYPE_INDEX) == TABLETYPE_INDEX)
#define IsTable(w)					((w & TABLETYPE_INDEX) != TABLETYPE_INDEX)
#define IsDummy(w)					((w & TABLETYPE_DUMMY) == TABLETYPE_DUMMY)
#define SwitchToTable(w)			(w & (~TABLETYPE_INDEX))
#define SwitchToIndex(w)			(w | TABLETYPE_INDEX)
#define SwitchToDummy(w)			(w | TABLETYPE_DUMMY)

#define IsSet(w, opt)		((w & opt) == opt)
*/



//----------------------------------------------------------------------------------
//for public
#define INVALID_DWORD_VALUE				((DWORD)-1)

#define FreeArrThenNil(p)				if(p) { delete[] p; p = NULL; }
#define FreeThenNil(p)					if(p) { delete p; p = NULL; }
#define ReleaseThenNil(p)				if(p) {	p->Release(); p = NULL; }
#define CloseFileThenNil(p)				if(p) {	fclose(p); p = NULL; }
#define InterfaceDuplication(p,q)			(p=q)->AddRef();



//----------------------------------------------------------------------------------
// inline VOID SBED_W(WORD &w)
// {
// #ifdef RS_BIG_ENDIAN
// 	w = RS_SWAPWORD(w);
// #endif
// };
// 
// inline VOID SBED_DW(DWORD &dw)
// {
// #ifdef RS_BIG_ENDIAN
// 	dw = SWAPDWORD(dw);
// #endif
// }





//==================================================================================
//maybe overwrite define
#ifndef NOTIMPL
#define NOTIMPL							{ return E_NOTIMPL; }
#endif

#ifndef ASSERT
#define ASSERT(x)						{ if (!(x)) { DebugBreak(); } }
#endif

#ifndef RFAILED_
#define RFAILED_(x, r)					{ if ( FAILED(x) ) { return (r); } }
#endif




//------------------------------------------------------------------------------------
//rot method
template<typename iT>
inline 
HRESULT create_iface_with_rot(IRERunningObjectTable* prot, const GUID& gid, iT** ppobj)
{
	RASSERT(prot && ppobj, E_UNEXPECTED);

	UTIL::com_ptr<IUnknown> punk;
	RFAILED(prot->GetObject(PID_ROT_ObjectLoader, &punk.m_p));

	UTIL::com_ptr<IREObjectLoader> pol;
	pol = punk;
	RASSERT(pol, E_UNEXPECTED);

	RFAILED(pol->CreateInstance(prot, gid, 0, re_uuidof(iT), (void**)ppobj));
	return S_OK;
}

template<typename iT>
inline 
HRESULT get_iface_with_rot(IRERunningObjectTable* prot, const GUID& gid, iT** ppobj)
{
	UTIL::com_ptr<IUnknown> punk;
	RFAILED(prot->GetObject(gid, &punk.m_p));
	RFAILED(punk->QueryInterface(re_uuidof(iT), (void**)ppobj));
	return S_OK;
}


//------------------------------------------------------------------------------------
//property method
inline 
PROPVARIANT create_re_property(VARENUM vt, VOID* pVal)
{
	PROPVARIANT var = PROPVARIANT();
	var.vt = vt;

	switch(vt)
	{
	case VT_UI2:
		var.uiVal = *(USHORT*)pVal;
		break;
	case VT_UI4:
		var.ulVal = *(ULONG*)pVal;
		break;
	case VT_INT:
		var.intVal = *(INT*)pVal;
		break;
	case VT_UINT:
		var.uintVal = *(UINT*)pVal;
		break;
	case VT_UNKNOWN:
		var.punkVal = (IUnknown*)pVal;
		break;
	case VT_LPSTR:
		var.pszVal = (LPSTR)pVal;
		break;
	case VT_LPWSTR:
		var.pwszVal = (LPWSTR)pVal;
		break;
	default:
		break;
	}
	return var;
}




//------------------------------------------------------------------------------------
// com ptr
typedef UTIL::com_ptr<IREProperty> IREPropertyPtr;
typedef UTIL::com_ptr<IUnknown> IUnknownPtr;
typedef UTIL::com_ptr<IREStream> IREStreamPtr;
typedef UTIL::com_ptr<IREStream> IREFilePtr;






#endif //_LIBLOADER_UTIL_H_


