#ifndef __CRANGE_H_
#define __CRANGE_H_

#if defined(_MSC_VER) && _MSC_VER > 1000
#pragma once
#endif

template <class T=long>
class CRange
{
private:
	T _l;
	T _r;
	typedef struct __tagrange
	{
		T l;
		T r;
		struct __tagrange * next;
		struct __tagrange * prev;
	}range;
	range *_phead;
	mutable range * _valid;
	mutable  range *_invalid;
	mutable  range *_invalid_prev;

public:
	// constructor
	inline CRange() {_l = 0; _r = 0; _phead=NULL; _valid=_phead; _invalid=_phead;}
	inline CRange(T l, T r)
	{
		if (l>r) _swap(l, r);
		_l = l; _r = r; _phead=NULL;
		_valid=_phead; _invalid=_phead;
	}
	inline void Init(T l, T r)
	{
		_free();
		if (l>r) _swap(l, r);
		_l = l; _r = r; _phead=NULL;
		_valid=_phead; _invalid=_phead;
	}

	// destructor
	inline ~CRange()
	{
		_free();
	}

	inline void AddRange(T l, T r)
	{
		_valid=_phead; _invalid=_phead;
		if (l>r) _swap(l, r);
		if (l<_l) l=_l;
		if (r>_r) r=_r;
		if (l == r) return;

		if (!_phead) 
		{
			_phead=(range *)RSMALLOC(sizeof(range));
			_phead->l = l;
			_phead->r = r;
			_phead->next = NULL;
			_phead->prev = NULL;
			return;
		}

		range *pr = _phead;
		
		if (r>=pr->l)
		{
			// find last effect R range
			while(pr->next && r>=pr->next->l) pr=pr->next;
			range *pl = pr;
			if (l<=pl->r)
			{	// find last effect L range from last effect R range
				while(pl->prev && l<=pl->prev->r) pl=pl->prev;
				// marage
				pl->l = pl->l<l ? pl->l : l;
				pl->r = pr->r>r ? pr->r : r;
				pl->next = pr->next;
				while(pl!=pr)
				{
					range *p = pr;
					pr=pr->prev;
					RSFREE(p);
				}
				if (pl->next) pl->next->prev = pl;
			}
			else //insert from pl
			{
				range *p=(range *)RSMALLOC(sizeof(range));
				p->l = l;
				p->r = r;
				p->next = pl->next;
				p->prev = pl;
				pl->next = p;
				if (p->next) p->next->prev =p;
			}
		}
		else	//insert from _phead
		{
			range *p=(range *)RSMALLOC(sizeof(range));
			p->l = l;
			p->r = r;
			p->next =_phead;
			p->prev = NULL;
			_phead->prev =p;
			_phead = p;
			return;
		}
	}	

	inline void DelRange(T l, T r)
	{
		_valid=_phead; _invalid=_phead;
		if (!_phead) return;
		if (l>r) _swap(l, r);
		if (l<_l) l=_l;
		if (r>_r) r=_r;
		if (l == r) return;
		// start
		range *pr = _phead;
		
		if (r>=pr->l)
		{
			// find last effect R range
			while(pr->next && r>=pr->next->l) pr=pr->next;
			range *pl = pr;
			if (l<=pl->r)
			{	// find last effect L range from last effect R range
				while(pl->prev && l<=pl->prev->r) pl=pl->prev;
				
				if (pl != pr)
				{
					//free nodes between pl and pr
					range *p = pl->next;
					while(p!=pr)
					{
						range *p2 =p;
						p=p->next;
						RSFREE(p2);
					}
					pl->next = pr;
					pr->prev = pl;

					// handle pl
					if (l<=pl->l) //del
					{
						pr->prev = pl->prev;
						if (pl->prev)
							pl->prev->next = pr;
						else
							_phead=pr;
						RSFREE(pl);
					}
					else
					{
						pl->r=l;
					}

					// handle pr
					if (r>=pr->r)	//del
					{
						if (pr->next) pr->next->prev=pr->prev;
						if (pr->prev) 
							pr->prev->next=pr->next;
						else
							_phead = pr->next;
						RSFREE(pr);
					}
					else
					{
						pr->l=r;
					}
				}

				else //pl==pr just effect one Node
				{
					if (l<=pl->l && r>=pl->r)	//del
					{
						if (pl->next)
							pl->next->prev = pl->prev;
						if (pl->prev)
							pl->prev->next = pl->next;
						else
							_phead = pl->next;
						RSFREE(pl);
					}
					else if (l>pl->l && r<pl->r) //split 2 unit
					{
						range *p = (range *)RSMALLOC(sizeof(range));
						p->l=r;
						p->r=pl->r;
						pl->r=l;
						p->prev = pl;
						p->next = pl->next;
						pl->next = p;
						if (p->next) p->next->prev = p;
					}
					else						//reduce
					{
						if (l>pl->l)
							pl->r=l;
						else
							pl->l=r;
					}
				}// pl==pr
			}// effect
		}
	}
	inline bool GetFristValidRange(T & t1, T & t2) const
	{
		if (!_phead) return false;
		_valid = _phead->next;
		t1 = _phead->l;
		t2 = _phead->r;
		return true;
	}

	inline bool GetNextValidRange(T & t1, T & t2) const
	{
		if (_valid == _phead) return false;
		if (!_valid) return false;
		t1 = _valid->l;
		t2 = _valid->r;
		_valid = _valid->next;
		return true;
	}
	inline bool GetFristInvalidRange(T & t1, T & t2) const
	{
		if (!_phead) return false;
		if (_phead->l == _l)
		{
			if (_phead->r == _r) return false;
			t1 = _phead->r;
			if (_phead->next) 
			{
				t2 = _phead->next->l;
				_invalid = _phead->next->next;
				if (!_invalid) _invalid_prev = _phead->next;
			}
			else
			{
				t2 = _r;
				_invalid = NULL;
				_invalid_prev = NULL;
			}
		}
		else
		{
			t1 = _l;
			t2 = _phead->l;
			if (!_phead->next) _invalid_prev = _phead;
			_invalid = _phead->next;
		}
		return true;
	}
	inline bool GetNextInvalidRange(T & t1, T & t2) const
	{
		if (_invalid == _phead) return false;
		if (!_invalid)
		{
			if (!_invalid_prev) return false;
			if (_invalid_prev->r == _r) return false;
			t1 = _invalid_prev->r;
			t2 = _r;
			_invalid_prev = NULL;
			return true;
		}
		else
		{
			t1 = _invalid->prev->r;
			t2 = _invalid->l;
			if(!_invalid->next) _invalid_prev = _invalid;
			_invalid = _invalid->next;
			
			return true;
		}
	}
private:
	inline void _swap(T & t1, T & t2) {T tmp=t1; t1=t2; t2=tmp; }
	inline void _free()
	{
		while(_phead)
		{
			range * p=_phead;
			_phead = _phead->next;
			RSFREE(p);
		}
	}
};


#endif //__CRANGE_H_
