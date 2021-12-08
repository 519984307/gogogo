include ../../Makefile.conf

CXXFLAGS=$(PUBCFLAGS) $(FNORTTI) -DUNIX  -DUNIXPLATFORM -D_NOUSESHM_  $(WXCONFIG)
INCS=$(RAVTOOLINC) -I. -I./rscommon
OBJS=Config.o Tree.o Node.o CheckSn.o CheckValid.o\
	CfgMutex.o shmem.o RavBase64.o mutex.o CfgApiImpl.o \
	hash.o unixrscfg.o regapi.o

ALLEXE=$(LIBDIR)/libravcfg$(EXTSO)
all: $(ALLEXE)

$(LIBDIR)/libravcfg$(EXTSO):$(OBJS)
	$(LDSO) -o  $@ $(OBJS) $(LRAVPUBLIB) $(EXTRALIB)

Config.o:	Config.cpp
	$(CXX) -c $(INCS) $(CXXFLAGS) Config.cpp

Tree.o:	Tree.cpp
	$(CXX) -c $(INCS) $(CXXFLAGS) Tree.cpp

Node.o:	Node.cpp
	$(CXX) -c $(INCS) $(CXXFLAGS) Node.cpp

CheckSn.o:	CheckSn.cpp
	$(CXX) -c $(INCS) $(CXXFLAGS)  CheckSn.cpp

CheckValid.o:	CheckValid.cpp
	$(CXX) -c $(INCS) $(CXXFLAGS) CheckValid.cpp

CfgMutex.o:	CfgMutex.cpp
	$(CXX) -c $(INCS) $(CXXFLAGS) CfgMutex.cpp

mutex.o:	mutex.cpp
	$(CXX) -c $(INCS) $(CXXFLAGS) mutex.cpp

shmem.o:	shmem.cpp
	$(CXX) -c $(INCS) $(CXXFLAGS) shmem.cpp

RavBase64.o:	RavBase64.cpp
	$(CXX) -c $(INCS) $(CXXFLAGS) RavBase64.cpp

CfgApiImpl.o:	CfgApiImpl.cpp
	$(CXX) -c $(INCS) $(CXXFLAGS) CfgApiImpl.cpp

hash.o:	hash.cpp
	$(CXX) -c $(INCS) $(CXXFLAGS) hash.cpp

unixrscfg.o:	unixrscfg.cpp
	$(CXX) -c $(INCS) $(CXXFLAGS) unixrscfg.cpp

regapi.o:	regapi.cpp
	$(CXX) -c $(INCS) $(CXXFLAGS) regapi.cpp

CheckSoftChange.o:	CheckSoftChange.cpp
	$(CXX) -c $(INCS) $(CXXFLAGS) CheckSoftChange.cpp

clean   :
	rm -f *.o $(ALLEXE)
