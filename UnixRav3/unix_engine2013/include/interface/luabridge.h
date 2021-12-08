#ifndef __LUA_WRAPPER__
#define __LUA_WRAPPER__

#include "../../lib3rd/luajit/lua.hpp"

enum HCLASS
{
	HC_TEMP		= 0,
	HC_GLOBAL	= 1,		// 
	HC_CONST	= 2,		// u can't close it
	HC_ALL		= -1,
};

//class ILuaObject
//{
//public:
//	STDMETHOD(GetRefId)() PURE;
//	STDMETHOD(GetLuaMachine)( IRELuaMachine** ppLua ) PURE;
//};

class IREObjectTable4Lua 
{
public:
	STDMETHOD(Open)( HANDLE hIn, LPHANDLE hpOut, HCLASS dwClass = HC_TEMP ) PURE;
	STDMETHOD(Alloc)( LPHANDLE hpOut, IUnknown * po, HANDLE hWant = NULL, HCLASS hClass = HC_TEMP ) PURE;
	STDMETHOD(Query)( HANDLE hInst, REFIID riid, IUnknown** ppv ) PURE;
	STDMETHOD(Close)( HANDLE hInst, HCLASS dwClass ) PURE;
	STDMETHOD(Clear)( HCLASS dwClass ) PURE;
};

class IRELuaMachine : public IUnknown, public IREObjectTable4Lua
{
public:
	RE_DECLARE_IID;
	STDMETHOD_(lua_State*, GetLuaState)() PURE;
};

// {F9645D86-48F1-44ff-AD90-7C84F5AE2F6B}
RE_DEFINE_IID( IRELuaMachine, "{F9645D86-48F1-44ff-AD90-7C84F5AE2F6B}",
			  0xf9645d86, 0x48f1, 0x44ff, 0xad, 0x90, 0x7c, 0x84, 0xf5, 0xae, 0x2f, 0x6b);

//////////////////////////////////////////////////////////////////////////

#endif