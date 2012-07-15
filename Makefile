CFLAGS=-Wall -g -I.
LDFLAGS=-g

X52LIB=libx52pro.so.0.1.1

all: $(X52LIB) x52output x52output.1.gz

$(X52LIB): x52pro.c
	${CC} $< ${CFLAGS} -shared -Wl,-soname,libx52pro.so.0 -fPIC -D_REENTRANT -L. -lusb -o $@

clean:
	-rm *.so* *.o x52output x52output.1.gz

install:
	install -D -m 644 x52pro.h $(DESTDIR)/usr/include/x52pro.h
	install -D -m 644 $(X52LIB) $(DESTDIR)/usr/lib/$(X52LIB)
	ln -sf $(X52LIB) $(DESTDIR)/usr/lib/libx52pro.so.0
	ln -sf libx52pro.so.0 $(DESTDIR)/usr/lib/libx52pro.so
	install -D -m 644 x52output.c $(DESTDIR)/usr/share/doc/libx52pro0/examples/x52output.c
	install -D x52output $(DESTDIR)/usr/bin/x52output
	install -D -m 644 99-x52pro.rules $(DESTDIR)/etc/udev/rules.d/99-x52pro.rules
	install -D -m 644 x52output.1.gz $(DESTDIR)/usr/share/man/man1/x52output.1.gz

x52output: x52output.o $(X52LIB) -lusb
x52output.1.gz: x52output.1
	gzip -c $< >$@
