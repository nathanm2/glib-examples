#include <glib-object.h>

typedef struct _Foo
{
    GObject parent;
    int value;
} Foo;

typedef struct _FooClass
{
    GObjectClass parent;

    void (*method)(Foo* foo);
} FooClass;

GType foo_get_type(void)
{
    static GType foo_type = 0;

    if(!foo_type) {
        static const GTypeInfo foo_info = {
            .class_size = sizeof(FooClass),
            .instance_size = sizeof(Foo)
        };

        foo_type = g_type_register_static(G_TYPE_OBJECT, "Foo", &foo_info, 0);
    }
    return foo_type;
}

#define FOO_TYPE (foo_get_type())

int main(int argc, char* argv[])
{
    Foo* foo = (Foo*)g_object_new(FOO_TYPE, NULL);
    foo->value = 42;
    return 0;
}
