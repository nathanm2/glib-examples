
.PHONY: clean all


GLIB_PREFIX := $(wildcard ~/build/glib)
CFLAGS = -g -O0 
CPPFLAGS = -I $(GLIB_PREFIX)/include/glib-2.0 -I $(GLIB_PREFIX)/lib/glib-2.0/include

all: gobject delayed children

gobject: gobject.o
	${CC} -o $@ ${inc} $(CFLAGS) $(CPPFLAGS) -L $(GLIB_PREFIX)/lib -Wl,-rpath=$(GLIB_PREFIX)/lib $< -lgobject-2.0 -lglib-2.0

delayed: delayed.o
	${CC} -o $@ ${inc} $(CFLAGS) $(CPPFLAGS) -L $(GLIB_PREFIX)/lib -Wl,-rpath=$(GLIB_PREFIX)/lib $< -lgobject-2.0 -lglib-2.0

children: children.o
	${CC} -o $@ ${inc} $(CFLAGS) $(CPPFLAGS) -L $(GLIB_PREFIX)/lib -Wl,-rpath=$(GLIB_PREFIX)/lib $< -lgobject-2.0 -lglib-2.0

clean:
	rm -f gobject delayed *.o
