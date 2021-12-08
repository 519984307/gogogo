include ../Makefile.conf

TARGET=$(LIB_DIR)/libravstorage.so
OBJS=storage.o \
	 cinifile.o \
	 Entry/settingentry.o \
	 SQLite3/sqlite3.o \
	 Entry/baseentry.o \
	 CPPSQLite/cppsqlite3.o \
	 Entry/virusinfoentry.o \
     Entry/logevententry.o \
     Entry/scheduleentry.o \
     Entry/whitelistentry.o \
     Entry/commeventlog.o \
     Entry/quarantinelog.o \
     Entry/netaccessentry.o \
     Entry/urlruleentry.o \
	 Entry/sourcelistentry.o

all:$(TARGET)

$(TARGET):$(OBJS)
	$(CXX) -shared $^ -o $@ 

%.o:%.cpp
	$(CXX) -c $< -o $@ -I. $(COMMON_CFLAG)

%.o:%.c
	$(CC) -c $< -o $@ -I. $(COMMON_CFLAG)

clean:
	rm -f $(TARGET) $(OBJS)
