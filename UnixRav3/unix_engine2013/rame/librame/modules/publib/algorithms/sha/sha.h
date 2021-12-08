#ifndef __ALGORITHM_SHA_INTERFACE_H__
#define __ALGORITHM_SHA_INTERFACE_H__

class SHA // Message Digest Algorithm SHA
{
public:
	virtual void  Init() = 0;
	virtual void  Update(const void *pvSrc, size_t cbSize) = 0;
	virtual void  Final() = 0;
	virtual const unsigned char* GetDigestStream() = 0;
	virtual size_t GetDigestSize() = 0;
};


#endif // __ALGORITHM_SHA_INTERFACE_H__
