#include <glib-object.h>


#define DMI_DIAG_FRAMEWORK_TYPE (dmi_diag_framework_get_type())
G_DECLARE_INTERFACE(DmiDiagFramework, dmi_diag_framework, DMI, DIAG_FRAMEWORK,
                    GObject);

struct _DmiDiagFrameworkInterface {
    GTypeInterface parent;

    int (*send_frame)(DmiDiagFramework * self, int value);
};

G_DEFINE_INTERFACE(DmiDiagFramework, dmi_diag_framework, G_TYPE_OBJECT);

static void
dmi_diag_framework_default_init(DmiDiagFrameworkInterface *iface)
{
    /* Add properties and signals here. */
}


int dmi_diag_framework_send_frame(DmiDiagFramework* self, int value)
{
    DmiDiagFrameworkInterface* iface;

    iface = DMI_DIAG_FRAMEWORK_GET_IFACE(self);
    return iface->send_frame(self, value);
}

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

static int
foo_send_frame(DmiDiagFramework *self,
               int value)
{
    return 42;
}

static void
foo_dmi_diag_framework_interface_init(gpointer g_iface,
                                      gpointer iface_data)
{
    DmiDiagFrameworkInterface *iface = g_iface;
    iface->send_frame = foo_send_frame;
}

GType foo_get_type(void)
{
    static GType foo_type = 0;

    if(!foo_type) {
        static const GTypeInfo foo_info = {
            .class_size = sizeof(FooClass),
            .instance_size = sizeof(Foo)
        };

        static const GInterfaceInfo diag_info = {
            .interface_init = foo_dmi_diag_framework_interface_init,
        };

        foo_type = g_type_register_static(G_TYPE_OBJECT, "Foo", &foo_info, 0);

        g_type_add_interface_static(foo_type, DMI_DIAG_FRAMEWORK_TYPE,
                                    &diag_info);
    }
    return foo_type;
}

#define FOO_TYPE (foo_get_type())
int main(int argc, char* argv[])
{
    Foo* foo = (Foo*)g_object_new(FOO_TYPE, NULL);
    foo->value = 42;
    dmi_diag_framework_send_frame((DmiDiagFramework*)foo, 42);
    return 0;
}
