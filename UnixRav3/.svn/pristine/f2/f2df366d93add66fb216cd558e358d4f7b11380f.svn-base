CC=gcc -g3
CXX=g++ -g3

CFLAGS=-I../CommonAPI/crypt -I../CommonAPI/zip
LDFLAGS=-lz

UPMAKER=UpgradeMaker 
PACKTOOLS=PackTools

UPMAKE_OBJS=upgrademaker.o ../CommonAPI/crypt/md5.o ../CommonAPI/zip/unixzip.o ../CommonAPI/zip/unzip.o
PACK_OBJS=ravpacksetup.o ../CommonAPI/crypt/md5.o ../CommonAPI/zip/unixzip.o ../CommonAPI/zip/unzip.o

all: $(UPMAKER) $(PACKTOOLS)

$(UPMAKER):$(UPMAKE_OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(PACKTOOLS):$(PACK_OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) 

%.o:%.cpp
	$(CXX) -o $@ -c $< $(CFLAGS)

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f $(UPMAKER) $(UPMAKE_OBJS) %$(PACKTOOLS) $(PACK_OBJS)
