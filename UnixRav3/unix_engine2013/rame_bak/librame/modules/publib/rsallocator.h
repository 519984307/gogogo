#ifndef _RSALLOCATOR_H_
#define _RSALLOCATOR_H_


#include "re_stdlib.h"

#if (_MSC_VER >= 1300 && !defined(PLATFORM_TYPE_NATIVE))


#include <cstddef>
#include <new>


namespace rsallocator {


//===== type traits ============================================================
struct true_type {};
struct false_type {};


template<class _Ty>
struct type_traits
{
	// Do not remove this member. It informs a compiler which
	// automatically specializes type_traits that this
	// type_traits template is special. It just makes sure that
	// things work if an implementation is using a template
	// called type_traits for something unrelated.
	typedef true_type	this_dummy_member_must_be_first;

	// The following restrictions should be observed for the sake of
	// compilers which automatically produce type specific specializations
	// of this class:
	// - You may reorder the members below if you wish
	// - You may remove any of the members below if you wish
	// - You must not rename members without making the corresponding
	//   name change in the compiler
	// - Members you add will be treated like regular members unless
	//   you add the appropriate support in the compiler.
	typedef false_type	has_trivial_default_constructor;
	typedef false_type	has_trivial_copy_constructor;
	typedef false_type	has_trivial_assignment_operator;
	typedef false_type	has_trivial_destructor;
	typedef false_type	is_POD_type; // plain old data
};

// Provide some specializations.

template<class _Ty>
struct type_traits<_Ty*>
{
	typedef true_type	has_trivial_default_constructor;
	typedef true_type	has_trivial_copy_constructor;
	typedef true_type	has_trivial_assignment_operator;
	typedef true_type	has_trivial_destructor;
	typedef true_type	is_POD_type;
};

template<class _Ty>
struct type_traits<const _Ty*>
{
	typedef true_type	has_trivial_default_constructor;
	typedef true_type	has_trivial_copy_constructor;
	typedef true_type	has_trivial_assignment_operator;
	typedef true_type	has_trivial_destructor;
	typedef true_type	is_POD_type;
};

template<>
struct type_traits<bool>
{
	typedef true_type	has_trivial_default_constructor;
	typedef true_type	has_trivial_copy_constructor;
	typedef true_type	has_trivial_assignment_operator;
	typedef true_type	has_trivial_destructor;
	typedef true_type	is_POD_type;
};

template<>
struct type_traits<char>
{
	typedef true_type	has_trivial_default_constructor;
	typedef true_type	has_trivial_copy_constructor;
	typedef true_type	has_trivial_assignment_operator;
	typedef true_type	has_trivial_destructor;
	typedef true_type	is_POD_type;
};

template<>
struct type_traits<signed char>
{
	typedef true_type	has_trivial_default_constructor;
	typedef true_type	has_trivial_copy_constructor;
	typedef true_type	has_trivial_assignment_operator;
	typedef true_type	has_trivial_destructor;
	typedef true_type	is_POD_type;
};

template<>
struct type_traits<unsigned char>
{
	typedef true_type	has_trivial_default_constructor;
	typedef true_type	has_trivial_copy_constructor;
	typedef true_type	has_trivial_assignment_operator;
	typedef true_type	has_trivial_destructor;
	typedef true_type	is_POD_type;
};

/*
template<>
struct type_traits<wchar_t>
{
	typedef true_type	has_trivial_default_constructor;
	typedef true_type	has_trivial_copy_constructor;
	typedef true_type	has_trivial_assignment_operator;
	typedef true_type	has_trivial_destructor;
	typedef true_type	is_POD_type;
};
*/

template<>
struct type_traits<short>
{
	typedef true_type	has_trivial_default_constructor;
	typedef true_type	has_trivial_copy_constructor;
	typedef true_type	has_trivial_assignment_operator;
	typedef true_type	has_trivial_destructor;
	typedef true_type	is_POD_type;
};

template<>
struct type_traits<unsigned short>
{
	typedef true_type	has_trivial_default_constructor;
	typedef true_type	has_trivial_copy_constructor;
	typedef true_type	has_trivial_assignment_operator;
	typedef true_type	has_trivial_destructor;
	typedef true_type	is_POD_type;
};

template<>
struct type_traits<int>
{
	typedef true_type	has_trivial_default_constructor;
	typedef true_type	has_trivial_copy_constructor;
	typedef true_type	has_trivial_assignment_operator;
	typedef true_type	has_trivial_destructor;
	typedef true_type	is_POD_type;
};

template<>
struct type_traits<unsigned int>
{
	typedef true_type	has_trivial_default_constructor;
	typedef true_type	has_trivial_copy_constructor;
	typedef true_type	has_trivial_assignment_operator;
	typedef true_type	has_trivial_destructor;
	typedef true_type	is_POD_type;
};

template<>
struct type_traits<LONG>
{
	typedef true_type	has_trivial_default_constructor;
	typedef true_type	has_trivial_copy_constructor;
	typedef true_type	has_trivial_assignment_operator;
	typedef true_type	has_trivial_destructor;
	typedef true_type	is_POD_type;
};

template<>
struct type_traits<ULONG>
{
	typedef true_type	has_trivial_default_constructor;
	typedef true_type	has_trivial_copy_constructor;
	typedef true_type	has_trivial_assignment_operator;
	typedef true_type	has_trivial_destructor;
	typedef true_type	is_POD_type;
};

template<>
struct type_traits<long long>
{
	typedef true_type	has_trivial_default_constructor;
	typedef true_type	has_trivial_copy_constructor;
	typedef true_type	has_trivial_assignment_operator;
	typedef true_type	has_trivial_destructor;
	typedef true_type	is_POD_type;
};

template<>
struct type_traits<unsigned long long>
{
	typedef true_type	has_trivial_default_constructor;
	typedef true_type	has_trivial_copy_constructor;
	typedef true_type	has_trivial_assignment_operator;
	typedef true_type	has_trivial_destructor;
	typedef true_type	is_POD_type;
};

template<>
struct type_traits<float>
{
	typedef true_type	has_trivial_default_constructor;
	typedef true_type	has_trivial_copy_constructor;
	typedef true_type	has_trivial_assignment_operator;
	typedef true_type	has_trivial_destructor;
	typedef true_type	is_POD_type;
};

template<>
struct type_traits<double>
{
	typedef true_type	has_trivial_default_constructor;
	typedef true_type	has_trivial_copy_constructor;
	typedef true_type	has_trivial_assignment_operator;
	typedef true_type	has_trivial_destructor;
	typedef true_type	is_POD_type;
};

template<>
struct type_traits<long double>
{
	typedef true_type	has_trivial_default_constructor;
	typedef true_type	has_trivial_copy_constructor;
	typedef true_type	has_trivial_assignment_operator;
	typedef true_type	has_trivial_destructor;
	typedef true_type	is_POD_type;
};
//============================================================ type traits =====



template<class T>
class alloc
{	// generic allocator for objects of class T
	typedef alloc<T>	self;

public:
	typedef    size_t	size_type;
	typedef ptrdiff_t	difference_type;
	typedef         T*	pointer;
	typedef         T&	reference;
	typedef   const T*	const_pointer;
	typedef   const T&	const_reference;
	typedef         T	value_type;

public:
	template<class U>
	struct rebind
	{	// convert a alloc<_Ty, _Policy> to a alloc<U, _Policy>
		typedef alloc<U>	other;
	};

public:
	alloc()
	{	// construct default allocator (do nothing)
	}

	alloc(const self&)
	{	// construct by copying (do nothing)
	}

	template<class U>
	alloc(const alloc<U>&)
	{	// construct from a related allocator (do nothing)
	}

	self& operator = (const self&)
	{	// assign from an allocator
		return *this;
	}

	template<class U>
	self& operator = (const alloc<U>&)
	{	// assign from a related allocator
		return *this;
	}

	~alloc()
	{	// destruct allocator (do nothing)
	}

public:
	pointer address(reference val) const
	{	// return address of mutable val
		return &val;
	}

	const_pointer address(const_reference val) const
	{	// return address of nonmutable val
		return &val;
	}

public:
	size_type max_size() const
	{	// estimate maximum array size
		size_type n = size_type(-1) / sizeof(value_type);
		return n ? n : size_type(1);
	}

public:
	pointer allocate(size_type n/*, typename rebind<void>::other::const_pointer hint = 0*/)
	{	// allocate array of n elements
		return (pointer)rs_malloc(n*sizeof(value_type));
	}

	void construct(pointer p, const_reference val)
	{	// construct object at p with val
		::new (p) value_type(val);;
	}

	void destroy(pointer p)
	{	// destroy object at p
		__destroy(p, type_traits<T>::has_trivial_destructor());
	}

	void deallocate(pointer p, size_type /* n */)
	{	// deallocate object at p, ignore n
		rs_free(p);
	}

private:
	void __destroy(pointer, true_type /* trivial type */) {}
	void __destroy(pointer p, false_type /* nontrivial type */) { p->~T(); }
};


template<>
class alloc<void>
{	// generic allocator for type void
	typedef void	T;

	typedef alloc<T>	self;

public:
	typedef    size_t	size_type;
	typedef ptrdiff_t	difference_type;
	typedef         T*	pointer;
	typedef   const T*	const_pointer;
	typedef         T	value_type;

public:
	template<class U>
	struct rebind
	{	// convert a alloc<_Ty, _Policy> to a alloc<U, _Policy>
		typedef alloc<U>	other;
	};

public:
	alloc()
	{	// construct default allocator (do nothing)
	}

	alloc(const self&)
	{	// construct by copying (do nothing)
	}

	template<class U>
	alloc(const alloc<U>&)
	{	// construct from a related allocator (do nothing)
	}

	self& operator = (const self&)
	{	// assign from an allocator
		return *this;
	}

	template<class U>
	self& operator = (const alloc<U>&)
	{	// assign from a related allocator
		return *this;
	}

	~alloc()
	{	// destruct allocator (do nothing)
	}
};


template<class T, class U>
inline
bool operator == (const alloc<T>&, const alloc<U>&)
{	// test for allocator equality (always true)
	return true;
}

template<class T, class U>
inline
bool operator != (const alloc<T>&, const alloc<U>&)
{	// test for allocator inequality (always false)
	return false;
}


} // namespace rsallocator


#else // #if (_MSC_VER >= 1300)


#include <memory>


namespace rsallocator { ;


template<class T>
class alloc : public std::allocator<T>
{	// generic allocator for objects of class T
	typedef std::allocator<T>	_MyBase;
public:
	typedef	typename _MyBase::size_type			size_type;
	typedef	typename _MyBase::difference_type	difference_type;
	typedef	typename _MyBase::pointer			pointer;
	typedef	typename _MyBase::const_pointer		const_pointer;
	typedef	typename _MyBase::reference			reference;
	typedef	typename _MyBase::const_reference	const_reference;
	typedef	typename _MyBase::value_type		value_type;
public:
	pointer allocate(size_type n, const void* = 0)
	{	// allocate array of n elements
		return (pointer)rs_malloc(n*sizeof(value_type));
	}
	char* _Charalloc(size_type n)
	{
		return (char*)rs_malloc(n);
	}
	void deallocate(/* pointer */ void *p, size_type /* n */)
	{	// deallocate object at p, ignore n
		rs_free(p);
	}
};


template<>
class alloc<void> : public std::allocator<void>
{	// generic allocator for type void
	typedef std::allocator<void>	_MyBase;
public:
	typedef	_MyBase::pointer			pointer;
	typedef	_MyBase::const_pointer	const_pointer;
	typedef	_MyBase::value_type		value_type;
};


template<class T, class U>
inline
bool operator == (const alloc<T>&, const alloc<U>&)
{	// test for allocator equality (always true)
	return true;
}

template<class T, class U>
inline
bool operator != (const alloc<T>&, const alloc<U>&)
{	// test for allocator inequality (always false)
	return false;
}
	

} // namespace rsallocator


#endif // #if (_MSC_VER >= 1300)


#endif // duplicate inclusion protection
