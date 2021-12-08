target=upgrade
objs=upgrademaker.o md5.o zip/unixzip.o

all:$(target)

$(target):$(objs)
	g++ -o $@ $^ -lz

%.o:%.cpp
	g++ -g3 -c $< -o $@ -Izip

clean:
	rm -f $(target) $(objs)
