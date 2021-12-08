include ../../mkpublic
include mkobjs

x86 x64 b32 b64 l32 l64: $(LIB)

$(LIB) : $(OBJS)
	$(AR) $(LIB) $(OBJS) 

.cpp.o :
	$(CXC) $(CFLAGS) -I. $(OPT)

.c.o :
	$(CC) $(CFLAGS) -I. $(OPT)

clean:
	rm $(OBJS)
	rm $(LIB)