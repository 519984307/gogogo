include ../../mkpublic
include mkobjs

x86 x64 b32 b64 l32 l64: $(SO)

$(SO): $(OBJS)
	$(LDSO) -o $@ $(OBJS) $(LINK_COMM)

.cpp.o :
	$(CXC) $(CFLAGS) -I. 

.c.o :
	$(CC) $(CFLAGS) -I. 

clean:
	rm -f $(SO)
	rm -f $(OBJS)

