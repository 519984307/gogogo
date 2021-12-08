#ifndef __PROP_TEMPLATE__
#define __PROP_TEMPLATE__

#include <map>

template<class T>
class TProperty_Impl
{
public:
	HRESULT set_property(const T& rpid, const PROPVARIANT *pv)
	{
		typename PROPMAP::iterator it = m_map.lower_bound(rpid);
		if(it != m_map.end() && it->first == rpid) 
		{ 
			RoClearVariant(&it->second.Variant); 
		}
		else 
		{ 
			it = m_map.insert(it, std::make_pair(rpid, PROPNODE())); 
		}
		HRESULT hr = S_OK;
		if( pv ) hr = RoSetVariant( &it->second.Variant, pv );
		if(FAILED(hr)) { m_map.erase(it); }
		return hr;
	}
	HRESULT get_property(const T& rpid, const PROPVARIANT **ppv)
	{
		typename PROPMAP::iterator it = m_map.find(rpid);
		RASSERT(m_map.end() != it, E_INVALIDARG);
		if( ppv ) *ppv = &it->second.Variant;
		return S_OK;
	}
	HRESULT delete_property(const T& rpid)
	{
		typename PROPMAP::iterator it = m_map.find(rpid);
		RASSERT(m_map.end() != it, E_INVALIDARG);
		HRESULT hr = RoClearVariant(&it->second.Variant);
		m_map.erase(it);
		return hr;
	}
	HRESULT clear_property()
	{
		for(typename PROPMAP::iterator it = m_map.begin(); it != m_map.end(); )
		{
			if( it->second.Mask & PROP_EXPLICIT_DEL )
			{
				++ it;
			}
			else
			{
				typename PROPMAP::iterator it2 = it;
				++ it;
				RoClearVariant(&it2->second.Variant);
				m_map.erase( it2 );
			}
		}
		return S_OK;
	}
	HRESULT mark_property( const T& rpid, SIZE_T uMask )
	{ 
		typename PROPMAP::iterator it = m_map.find(rpid);
		RASSERT(m_map.end() != it, E_INVALIDARG);
		it->second.Mask = uMask;
		return S_OK;
	}
	HRESULT reset() 
	{
		for(typename PROPMAP::iterator it = m_map.begin(); it != m_map.end(); ++ it )
		{
			RoClearVariant(&it->second.Variant);
		}
		m_map.clear();
		return S_OK;
	}
	HRESULT move_property( IREPropertyEx * , const T&  )
	{ 
		return E_FAIL;
		//typename PROPMAP::iterator it = m_map.find(rpid);
		//RASSERT(m_map.end() != it, E_INVALIDARG);
		//prop->SetProperty( rpid, &(it->second.Variant) );
		//prop->MarkProperty( rpid, it->second.Mask );
		//delete_property(rpid);
		//return S_OK;
	}
public:
	~TProperty_Impl() { reset(); }
private:
	struct PROPNODE
	{
		SIZE_T		Mask;
		PROPVARIANT	Variant;
		PROPNODE() : Mask(0), Variant()
		{
			Variant.vt = VT_EMPTY;
		}
	};
	typedef std::map<T, PROPNODE>	PROPMAP;
	PROPMAP	m_map;
};

#endif //__RECOMP_PROPERTY_TEMPLATE_H__
