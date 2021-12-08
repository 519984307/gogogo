include ../Makefile.conf

DEFINE=-DCOMMON_STRING_API -DCOMMON_HASH_API -DCOMMON_OS_API -DCOMMON_PATH_API -DCOMMON_JSON_API -DCOMMON_LOG_API -DCOMMON_ZIP_API
CFLAGS=-fPIC -I../ThirdPart/include/dbus-1.0 -I../ThirdPart/lib/dbus-1.0/include $(RPC_TYPE) $(DEFINE)
LDFLAGS=-L../ThirdPart/lib -lravssl -lravcrypto -lravz

TARGET=$(LIB_DIR)/libravcommon.so
OBJS=commonapi.o \
    crypt/base64.o \
    crypt/md5.o \
    crypt/rc4.o \
    json/json_reader.o \
    json/json_value.o \
    json/json_writer.o \
	zip/unixzip.o \
	zip/unzip.o \
	handler.o \
	communicatebundle.o \
    configfile.o

all:$(TARGET)

$(TARGET):$(OBJS)
	$(CXX) -shared -o $@ $^ $(LDFLAGS)

%.o:%.cpp
	$(CXX) -c $< -o $@ -I. $(COMMON_CFLAG) -I$(EXTEND_INC) $(CFLAGS)

%.o:%.inl
	$(CXX) -c $< -o $@ -I. $(COMMON_CFLAG) -I$(EXTEND_INC)

clean:
	rm -f $(TARGET) $(OBJS)
