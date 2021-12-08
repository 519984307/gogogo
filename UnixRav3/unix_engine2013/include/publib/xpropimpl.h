#ifndef __RAW_XPROP_IMPL__
#define __RAW_XPROP_IMPL__

#include <string>
#include <map>

class XPropImpl
{
protected:
	enum
	{
		PROP_SAVE = 1,
	};
	struct VALUE
	{
		xv::xvalue_t	v;
		ULONG			f;
		VALUE() : f(0) {};
	};
	typedef std::map<std::string,VALUE>	PROPMAP;
	PROPMAP	_propmap;
public:
	HRESULT SetProp( LPCSTR pcName, xv::Value& val, BOOL bNeedSave = FALSE )
	{
		VALUE tmp; tmp.f = bNeedSave?PROP_SAVE:0;
		tmp.v = val;
		_propmap[pcName] = tmp;
		return S_OK;
	}
	HRESULT GetProp( LPCSTR pcName, xv::Value& val )
	{
		PROPMAP::iterator it = _propmap.find( std::string(pcName) );
		if( it == _propmap.end() ) return E_FAIL;
		xv::xvalue_t v = it->second.v;
		if( !v.get(val) ) return E_FAIL;
		return S_OK;
	}
	HRESULT DelProp( LPCSTR pcName )
	{
		PROPMAP::iterator it = _propmap.find( std::string(pcName) );
		if( it == _propmap.end() ) return E_FAIL;
		_propmap.erase(it);
		return S_OK;
	}
	HRESULT EnableSave( LPCSTR pcName, BOOL bSave )
	{
		PROPMAP::iterator it = _propmap.find( std::string(pcName) );
		if( it == _propmap.end() ) return E_FAIL;
		if( bSave ) it->second.f |= PROP_SAVE;
		else it->second.f &= (~PROP_SAVE);
		return S_OK;
	}
	HRESULT Clear()
	{
		_propmap.clear();
		return S_OK;
	}
	HRESULT CoQueryProp( LPCSTR pcName, REFIID riid, void** ppv )
	{
		xv::xvalue_t val;
		RFAILED( GetProp(pcName,val) );
		if( !val.checkType( xv::dUnknown ) )
			return E_FAIL;
		if( !val.punk ) return E_FAIL;
		return val.punk->QueryInterface( riid, ppv );
	}
	HRESULT CoSetProp( LPCSTR pcName, IUnknown* ppunk )
	{
		xv::xvalue_t val;
		if( !val.set(ppunk) ) return E_FAIL;
		return SetProp(pcName,val);
	}
	HRESULT Save( xv::Output * pout )
	{
		xv::xvalue_t tmp;
		refp<xv::vObject> obj = tmp.asObject();
		RASSERT( obj, E_OUTOFMEMORY );
		PROPMAP::iterator it = _propmap.begin();
		for( ; it != _propmap.end(); ++ it )
		{
			VALUE item = it->second;
			if( 0 == (item.f & PROP_SAVE) )
				continue;
			if( item.v.checkType(xv::dUnknown) )
				continue;
			if( !obj->set( it->first.c_str(), item.v ) )
				return E_FAIL;
		}
		if( !obj->length() ) return S_OK;
		return tmp.save(*pout) ? S_OK : E_FAIL;
	}
	HRESULT Load( LPCVOID bp, SIZE_T cb )
	{
		xv::xvalue_t tmp;
		RASSERT( tmp.load( bp, cb ), E_FAIL );
		RASSERT( tmp.checkType( xv::dObject ), E_FAIL );
		refp<xv::vObject> obj = tmp.asObject();
		RASSERT( obj, E_OUTOFMEMORY );
		RASSERT( obj->enum_it( XPropImpl::_load_xvalue, this ), E_FAIL );
		return S_OK;
	}
	bool load_xvalue( const char * name, xv::Value & item, bool )
	{
		VALUE tmp;
		tmp.f = PROP_SAVE;
		tmp.v = item;
		_propmap[name] = tmp;
		return true;
	}
	static bool _load_xvalue( const char * name, xv::Value & item, bool last, void * context )
	{
		return ((XPropImpl*)context)->load_xvalue(name,item,last);
	}
};

#endif
