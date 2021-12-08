CC=g++
OBJS=caja-rav.o rav-module.o

PLATFORM=aarch64-linux-gnu
TARGET=libcaja-rav.so

CAJA_CFLAGS = -pthread -I/usr/include/caja -I/usr/include/glib-2.0 -I/usr/lib/$(PLATFORM)/glib-2.0/include -I/usr/include/gtk-2.0 -I/usr/lib/$(PLATFORM)/gtk-2.0/include -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/pango-1.0 -I/usr/include/gio-unix-2.0/ -I/usr/include/freetype2 -I/usr/include/pixman-1 -I/usr/include/libpng12 -I/usr/include/harfbuzz -I../ThirdPart/include/dbus-1.0 -I../ThirdPart/lib/dbus-1.0/include  
CAJA_EXTENSION_DIR = /usr/lib/x86_64-linux-gnu/caja/extensions-2.0
CAJA_LIBS = -lcaja-extension -lgtk-x11-2.0 -lgdk-x11-2.0 -latk-1.0 -lpangoft2-1.0 -lpangocairo-1.0 -lgdk_pixbuf-2.0 -lcairo -lpango-1.0 -lfontconfig -lfreetype -lgio-2.0 -lgobject-2.0 -lglib-2.0 -ldbus-1 

all:$(TARGET)

$(TARGET):$(OBJS)
	$(CC) -shared -o $@ $^ $(CAJA_LIBS)

%.o:%.c
	$(CC) -fPIC -c $< -o $@ $(CAJA_CFLAGS)

clean:
	rm -f $(OBJS) $(TARGET)
