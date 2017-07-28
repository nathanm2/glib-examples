
.PHONY: clean

CFLAGS = -g -O0 
CPPFLAGS = -I /home/nmiller/build/glib/include/glib-2.0 -I /home/nmiller/build/glib/lib/glib-2.0/include

gobject: gobject.o
	${CC} -o $@ ${inc} $(CFLAGS) $(CPPFLAGS) -L /home/nmiller/build/glib/lib -Wl,-rpath=/home/nmiller/build/glib/lib $< -lgobject-2.0 -lglib-2.0

clean:
	gobject rm *.o
