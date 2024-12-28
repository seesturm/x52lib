CFLAGS := -Wall -g -I. $(shell pkg-config --cflags libusb-1.0)
LDFLAGS := -g
LDLIBS := $(shell pkg-config --libs libusb-1.0)

INSTALL=install
INSTALL_DATA=$(INSTALL) -m644

X52LIB := libx52pro.so.0.2.0

all: $(X52LIB) x52output x52output.1.gz

$(X52LIB): x52pro.c
	$(CC) $< $(CFLAGS) -shared -Wl,-soname,libx52pro.so.0 -fPIC -D_REENTRANT -L. $(LDLIBS) -o $@

clean:
	-rm *.so* *.o x52output x52output.1.gz

install:
	$(INSTALL_DATA) -D x52pro.h $(DESTDIR)/usr/include/x52pro.h
	$(INSTALL_DATA) -D $(X52LIB) $(DESTDIR)/usr/lib/$(X52LIB)
	ln -sf $(X52LIB) $(DESTDIR)/usr/lib/libx52pro.so.0
	ln -sf libx52pro.so.0 $(DESTDIR)/usr/lib/libx52pro.so
	$(INSTALL_DATA) -D x52output.c $(DESTDIR)/usr/share/doc/libx52pro0/examples/x52output.c
	$(INSTALL) -D x52output $(DESTDIR)/usr/bin/x52output
	$(INSTALL_DATA) -D 99-x52pro.rules $(DESTDIR)/lib/udev/rules.d/99-x52pro.rules
	$(INSTALL_DATA) -D x52output.1.gz $(DESTDIR)/usr/share/man/man1/x52output.1.gz
	$(INSTALL_DATA) -D x52pro.pc $(DESTDIR)/usr/lib/${DEB_HOST_MULTIARCH}/pkgconfig/x52pro.pc
	$(INSTALL_DATA) -D at.hasenleithner.plasma.x52pro.metainfo.xml $(DESTDIR)/usr/share/metainfo/at.hasenleithner.plasma.x52pro.metainfo.xml


x52output: x52output.o $(X52LIB)
x52output.1.gz: x52output.1
	gzip -c $< >$@
