#ifndef __SMARTPTR_H__
#define __SMARTPTR_H__

/*
//this class is used for class' pointer's redirection
//the class' offset is provided by outside input, the default value is 0
template <class T>
class SmartPtr
{
private:
	long m_lOffset;
	T *m_pClass;
public:
	//constructor
	SmartPtr(T *pRealClass, long lOffset = 0)
	{
		m_pClass = pRealClass;
		m_lOffset = lOffset;
	}

	//destructor
	~SmartPtr()
	{
		m_pClass = 0;
		m_lOffset = 0;
	}

	//get address operator
	T* operator->() const
	{
		if(!m_pClass)
			return NULL;
		return (T*)((unsigned char *)m_pClass + m_lOffset);
	}

	//get value operator
	T& operator*() const
	{
		return *(T*)((unsigned char *)m_pClass + m_lOffset);
	}

	//assignment operator
	SmartPtr<T>& operator=(const SmartPtr<T>& tt)
	{
		if(this != &tt)
		{
			if( m_pClass != tt.m_pClass )
				m_pClass = tt.m_pClass;
			return (*this);
		}
	}
};
*/
template <class T, void** p>
class BasePtr
{
private:	
public:
	unsigned int m_lOffset;
	//类型转换
	inline operator T*()
	{
		return (T*)(m_lOffset==0 ? 0 : ((char *)p[0] + m_lOffset));
	}
	//转化成其他类型
//	inline operator unsigned char*()
//	{
//		return (unsigned char*)((char *)p[0] + m_lOffset);
//	}
// Deleted by LauranceZhang 2004-8-3 14:27:37 < 
/*
	inline operator unsigned long*()
	{
		return (unsigned long*)((char *)p[0] + m_lOffset);
	}
*/
// Deleted by LauranceZhang 2004-8-3 14:27:37 > 
	//赋值操作
	inline BasePtr<T, p>& operator =(T*pTT)
	{
		if(pTT)
			m_lOffset = (char *)pTT - (char *)p[0];
		else
			m_lOffset = 0;
		return *this;
	}
	//指针操作
//	inline T* operator ->()
//	{
//		return operator T*();
//	}
	inline bool IsNull()
	{
		return m_lOffset==0?1:0;
	}
//	inline SetNull()
//	{
//		m_lOffset = 0;
//	}
	inline void Swap()
	{
		RS_SWAPDWORDEX(m_lOffset);
	}
};

template <class T, void** p>
class ClassPtr
{
private:
	unsigned int m_lOffset;
public:
	//类型转换
	inline operator T*()
	{
		return (T*)(m_lOffset==0 ? 0 : ((char *)p[0] + m_lOffset));
#ifdef _DEBUG
//		printf("\n%x<-->%x", (char *)p[0], m_lOffset);
#endif
	}
	//赋值操作
	inline ClassPtr<T, p>& operator =(T*pTT)
	{
		if(pTT)
			m_lOffset = (char *)pTT - (char *)p[0];
		else
			m_lOffset = 0;
#ifdef _DEBUG
//		printf("\n%x--%x",(char *)pTT, (char *)p[0]);
#endif
		return *this;
	}
	//==和!=操作
/*
	inline RS_BOOL operator != (T*pTT)
	{
		if(pTT)
		{
			RS_LONG lOffset = (char *)pTT - (char *)p[0];
			return (lOffset == m_lOffset)? RS_FALSE: RS_TRUE;
		}
		return (m_lOffset = 0)? RS_FALSE: RS_TRUE;
	}
	inline RS_BOOL operator == (T*pTT)
	{
		if(pTT)
		{
			RS_LONG lOffset = (char *)pTT - (char *)p[0];
			return (lOffset != m_lOffset)? RS_FALSE: RS_TRUE;
		}
		return (m_lOffset != 0)? RS_FALSE: RS_TRUE;
	}
*/
	//指针操作
	inline T* operator ->()
	{
		return operator T*();
	}
	inline long GetOffset()
	{
		return m_lOffset;
	}
	inline bool IsNull()
	{
		return m_lOffset==0?1:0;
	}
//	inline SetNull()
//	{
//		m_lOffset = 0;
//	}
	inline void Swap()
	{
		RS_SWAPDWORDEX(m_lOffset);
	}
};

#endif
