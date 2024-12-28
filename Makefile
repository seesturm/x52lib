.PHONY: all doxygen clean

CC := gcc
BUILDFLAGS := -g -O0
CFLAGS := -Wall -Wextra -Wpedantic -std=c99 $(BUILDFLAGS) -I.
LDFLAGS := $(BUILDFLAGS)
USBINC := $(shell pkg-config --cflags libusb-1.0)
USBLIB := $(shell pkg-config --libs libusb-1.0)

X52LIB := libx52pro.so.0.2.0

all: $(X52LIB) x52output x52output.1.gz

$(X52LIB): x52pro.c
	$(CC) $< $(CFLAGS) $(USBINC) -shared -Wl,-soname,libx52pro.so.0 -fPIC -D_REENTRANT -L. $(USBLIB) -o $@

clean:
	-rm *.so* *.o x52output x52output.1.gz
	-rm -R build

install:
	install -D -m 644 x52pro.h $(DESTDIR)/usr/include/x52pro.h
	install -D -m 644 $(X52LIB) $(DESTDIR)/usr/lib/$(X52LIB)
	ln -sf $(X52LIB) $(DESTDIR)/usr/lib/libx52pro.so.0
	ln -sf libx52pro.so.0 $(DESTDIR)/usr/lib/libx52pro.so
	install -D -m 644 x52output.c $(DESTDIR)/usr/share/doc/libx52pro0/examples/x52output.c
	install -D x52output $(DESTDIR)/usr/bin/x52output
	install -D -m 644 99-x52pro.rules $(DESTDIR)/lib/udev/rules.d/99-x52pro.rules
	install -D -m 644 x52output.1.gz $(DESTDIR)/usr/share/man/man1/x52output.1.gz
	install -D -m 644 x52pro.pc $(DESTDIR)/usr/lib/${DEB_HOST_MULTIARCH}/pkgconfig/x52pro.pc

x52output: x52output.o $(X52LIB)
x52output.1.gz: x52output.1
	gzip -c $< >$@

doxygen:
	doxygen
