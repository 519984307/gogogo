#ifndef __ATOMIC_T_H_H__
#define __ATOMIC_T_H_H__

#include "syncost.h"

template<class T>
class atomic_t{
	syncost::FastMutex _look;
	T _val;
public:
	atomic_t(int v):_val(v){
		_look.init(0,0);
	}
	atomic_t():_val(0){
		_look.init(0,0);
	}
	T load(){
		return _val;
	}
	T store(T v){
		syncost::MutexScope scope(_look);
		_val = v;
		return _val;
	} 
	T operator = (T v){
		syncost::MutexScope scope(_look);
		_val = v;
		return _val;
	}
	T & operator ++ (){
		syncost::MutexScope scope(_look);
		_val++;
		return _val;
	}
	T & operator -- (){
		syncost::MutexScope scope(_look);
		_val--;
		return _val;
	}
	const T operator ++ (int v){
		syncost::MutexScope scope(_look);
		_val++;
		return _val;
	}
	const T operator -- (int v){
		syncost::MutexScope scope(_look);
		_val--;
		return _val;
	}
	operator T () const {
		return _val;
	}
};

#endif //__ATOMIC_T_H_H__
