#ifndef __RE_LRU_MAP__
#define __RE_LRU_MAP__

#include <map>

template < class K, class V >
struct LruMap
{
	struct Callback
	{
		virtual bool PreObsolete( const K & key ) = 0;
		virtual bool OnEnumEntry( const K & key, V& val, void * context ) = 0;
	};

	struct KeyNode 
	{
		K			_k;
		KeyNode *	vnext;
		KeyNode *	vprev;

		KeyNode( K & k ) : _k(k), vnext(0), vprev(0) {};

		bool operator < ( const KeyNode & r ) const 
		{
			return _k < r._k;
		}

	} ;

	typedef std::map< KeyNode, V > _InLruMap;

	size_t		MAX_ENTRIES;

	LruMap( size_t Room = 100 ) 
		: MAX_ENTRIES(Room)
		, entFirst(0)
		, entLast(0)
		, cbObsolete(0)
	{

	}

	KeyNode *	entFirst;
	KeyNode *	entLast;
	Callback*	cbObsolete;

	_InLruMap	mapStore;

	size_t Count()
	{
		return mapStore.size();
	}

	bool Remove( K & k )
	{
		KeyNode kn(k);
		_InLruMap::iterator it = mapStore.find(kn);
		if( it == mapStore.end() ) 
			return false;
		visit_remove( (KeyNode&)(it->first) );
		mapStore.erase( it );
		return true;
	}

	bool Pin( K & k, V & v )
	{
		KeyNode kn(k);
		{
			_InLruMap::iterator it = mapStore.find(kn);
			if( it != mapStore.end() ) 
			{
				it->second = v;
				visit_append( (KeyNode&)(it->first));
				return true;
			}
		}
		{
			if( mapStore.size() >= MAX_ENTRIES )
			{
				if( !reject() ) return false;
			}
			_InLruMap::value_type pr( kn, v );
			_InLruMap::_Pairib ib = mapStore.insert( pr );
			if( ! ib.second ) return false;
			_InLruMap::iterator it = ib.first;
			visit_append( (KeyNode&)(it->first) );
		}
		return true;
	}

	bool Query( K & k, V & val )
	{
		KeyNode kn(k);
		_InLruMap::iterator it = mapStore.find( kn );
		if( it == mapStore.end() )
			return false;
		val = it->second;
		visit_append( (KeyNode&)(it->first) );
		return true;
	}

	void Clear()
	{
		entLast = 0;
		entFirst = 0;
		mapStore.clear();
	}

	bool Enum( void * context )
	{
		if( !cbObsolete ) return false;
		_InLruMap::iterator it = mapStore.begin();
		for( ; it != mapStore.end(); ++ it )
		{
			if( !cbObsolete->OnEnumEntry( it->first._k, it->second, context ) )
				break;
		}
		return true;
	}

	void SetMAX( size_t sMax )
	{
		MAX_ENTRIES = sMax;
	}
	void SetCallback( Callback* pcb )
	{
		cbObsolete = pcb;
	}

	// 
	bool pre_reject( const K & k )
	{
		if( !cbObsolete ) return true;
		return cbObsolete->PreObsolete( k );
	}
protected:

	bool reject()
	{
		if( !entLast ) return false;
		KeyNode * todel = entLast;
		while( todel )
		{
			if( pre_reject( todel->_k ) ) 
				break;
			todel = todel->vprev;
		}
		if( !todel ) return false;

		return Remove( todel->_k );
	}
	//bool reject()
	//{
	//	if( !entFirst ) return false;
	//	KeyNode * todel = entFirst;
	//	entFirst = entLast->vnext;
	//	if( !entFirst ) 
	//		entLast = 0;
	//	else
	//		entFirst->vprev = 0;

	//	_InLruMap::iterator it = mapStore.find( *todel );
	//	if( it == mapStore.end() ) 
	//		return false;
	//	on_reject( it->first._k, it->second );
	//	mapStore.erase( it );
	//	return true;
	//}

	void visit_remove( KeyNode & knode )
	{
		// cut
		KeyNode * prev = knode.vprev;
		KeyNode * next = knode.vnext;

		if( prev ) prev->vnext = next;
		if( next ) next->vprev = prev;

		if( entFirst == &knode ) entFirst = next;
		if( entLast == &knode ) entLast = prev;

		knode.vnext = 0;
		knode.vprev = 0;
	}

	void visit_append( KeyNode & knode )
	{
		visit_remove( knode );
		//// cut
		//KeyNode * prev = knode.vprev;
		//KeyNode * next = knode.vnext;

		//if( prev ) prev->vnext = next;
		//if( next ) next->vprev = prev;

		//if( entFirst == &knode ) entFirst = next;
		//if( entLast == &knode ) entLast = prev;

		//knode.vnext = 0;
		//knode.vprev = 0;

		//////////////////////////////////////////////////////////////////////////

		// insert to head, if empty
		if( !entFirst ) 
		{
			entFirst = &knode;
			entLast = &knode;
			return;
		}

		// insert to head
		entFirst->vprev = &knode;
		knode.vnext = entFirst;
		entFirst = &knode;

		return;
	}

};


#endif