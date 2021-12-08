include ../../mkpublic
include mkobjs

x86 x64 b32 b64 l32 l64: $(LIB)

$(LIB): $(OBJS)
	$(AR) $(LIB) $(OBJS)

.cpp.o :
	$(CXC) $(CFLAGS) -I. -I$(INCLUDE)

.c.o :
	$(CC) $(CFLAGS) -I. -I$(INCLUDE)

clean:
	rm -f $(LIB)
	rm -f $(OBJS)
