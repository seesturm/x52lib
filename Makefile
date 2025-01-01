.PHONY: all doxygen clean

# defaults
CC ?= gcc
CFLAGS ?= -Wall -Wextra -Wpedantic -std=c99 -g -O0
LDFLAGS ?= -g
INSTALL ?= install

# package specific flags
USBINC := $(shell pkg-config --cflags libusb-1.0)
USBLIB := $(shell pkg-config --libs libusb-1.0)
VERSION := 0.2.0
X52LIB := libx52pro.so.$(VERSION)

INSTALL_DATA := $(INSTALL) -m644
LIBDST := $(DESTDIR)/usr/lib

all: $(X52LIB) x52output x52output.1.gz

$(X52LIB): x52pro.o
	$(CC) $< $(LDFLAGS) -shared -Wl,-soname,libx52pro.so.0 $(USBLIB) -o $@

x52pro.o_CFLAGS := $(USBINC) -fPIC -D_REENTRANT
x52output.o_CFLAGS := -I.

%.o: %.c
	$(CC) $(CFLAGS) $($@_CFLAGS) -c -o $@ $<

clean:
	-$(RM) *.so* *.o x52output x52output.1.gz
	-$(RM) -R build

install:
	$(INSTALL_DATA) -D x52pro.h $(DESTDIR)/usr/include/x52pro.h
	$(INSTALL_DATA) -D $(X52LIB) $(LIBDST)/$(X52LIB)
	ln -sf $(X52LIB) $(LIBDST)/libx52pro.so.0
	ln -sf libx52pro.so.0 $(LIBDST)/libx52pro.so
	$(INSTALL_DATA) -D x52pro.pc $(LIBDST)/pkgconfig/x52pro.pc
	$(INSTALL_DATA) -D x52output.c $(DESTDIR)/usr/share/doc/libx52pro0/examples/x52output.c
	$(INSTALL_DATA) -D x52output.1.gz $(DESTDIR)/usr/share/man/man1/x52output.1.gz
	$(INSTALL) -D x52output $(DESTDIR)/usr/bin/x52output
	$(INSTALL_DATA) -D 60-x52pro.rules $(DESTDIR)/lib/udev/rules.d/60-x52pro.rules

x52output: x52output.o $(X52LIB)
x52output.1.gz: x52output.1
	gzip -c $< >$@

doxygen:
	doxygen

# Create symbolic link for local testing
libx52pro.so.0: $(X52LIB)
	ln -s $< $@
