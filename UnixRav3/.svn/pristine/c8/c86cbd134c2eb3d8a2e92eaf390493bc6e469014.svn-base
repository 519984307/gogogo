include ../../Makefile.conf

CFLAGS=-DCHINESE -fPIC -I. \
	   -I../../BaseController \
	   -I../../CommonAPI \
	   -I../../Storage \
	   -I../../ScanEngine \
	   -I../../Upgrade \
	   -I../../ThirdPart/include 
LDFLAGS=-L../../ThirdPart/lib -lravncurses -lravcrypto -lravssl -lravz -lravcurl -lraviconv \
		-L../../lib -lravstorage -lravcommon -lravbase -lravscanengine \
		-L../../unix_engine2013/rame.bin -lrxruntim -lrxmss -lrxcoml -lrxffr -lrxsig64

TARGET=../../bin/RavConsole
OBJS=main.o \
	 view.o \
	 textview.o \
	 window.o \
	 screen.o \
	 menu.o \
	 titleview.o \
	 dialog.o \
	 listview.o \
	 settingview.o \
	 editview.o \
	 optionview.o \
	 aboutview.o

all:$(TARGET)

$(TARGET):$(OBJS)
	$(CXX) -o $@ $^ -lpthread $(LDFLAGS)

%.o:%.cpp
	$(CXX) -c $< -o $@ $(CFLAGS)

clean:
	rm -f $(TARGET) $(OBJS)
