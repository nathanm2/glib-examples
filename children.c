#include <glib-object.h>
#include <stdio.h>

/** Resident Set Size: 2500/2800 vs 1128*/
/** Private: 184 vs 88*/
/* Base */
#define X_TYPE_BASE x_base_get_type()
G_DECLARE_DERIVABLE_TYPE(XBase, x_base, X, BASE, GObject);

struct _XBaseClass
{
    GObjectClass parent_class;

    void (*foo)(XBase* base);
};

void foo(gpointer obj)
{
    if(X_IS_BASE(obj)) {
        X_BASE_GET_CLASS(obj)->foo((XBase*)obj);
    }
}

G_DEFINE_TYPE(XBase, x_base, G_TYPE_OBJECT);

void x_base_foo(XBase* base)
{
    printf("x_base_foo\n");
}

void x_base_init(XBase* self)
{
    /* Add data initialization here. */
}

void x_base_finalize(GObject* self)
{
    printf("x_base_finalize (%s)\n", G_OBJECT_TYPE_NAME(self));
    G_OBJECT_CLASS(x_base_parent_class)->finalize(self);
}

void x_base_class_init(XBaseClass* klass)
{
    klass->foo = x_base_foo;
    G_OBJECT_CLASS(klass)->finalize = x_base_finalize;
}

/* Child 1 */
#define X_TYPE_CHILD x_child_get_type()
G_DECLARE_DERIVABLE_TYPE(XChild, x_child, X, CHILD, XBase);

struct _XChildClass
{
    XBaseClass parent;
};

G_DEFINE_TYPE(XChild, x_child, X_TYPE_BASE);

void x_child_foo(XBase* base)
{
    printf("x_child_foo\n");
}

void x_child_init(XChild* self)
{
    /* Add data initialization here. */
}

void x_child_class_init(XChildClass* klass)
{
    XBaseClass* xb_class = X_BASE_CLASS(klass);
    xb_class->foo = x_child_foo;
}

/* Grandchild */
#define X_TYPE_GRANDCHILD x_grandchild_get_type()
G_DECLARE_DERIVABLE_TYPE(XGrandchild, x_grandchild, X, GRANDCHILD, XChild);

struct _XGrandchildClass
{
    XChildClass parent;
};

G_DEFINE_TYPE(XGrandchild, x_grandchild, X_TYPE_CHILD);

void x_grandchild_foo(XBase* base)
{
    printf("x_grandchild_foo\n");
}

void x_grandchild_init(XGrandchild* self)
{
    /* Add data initialization here. */
}

void x_grandchild_finalize(GObject* self)
{
    printf("x_grandchild_finalize\n");
    G_OBJECT_CLASS(x_grandchild_parent_class)->finalize(self);
}

void x_grandchild_class_init(XGrandchildClass* klass)
{
    XBaseClass* xb_class = X_BASE_CLASS(klass);
    xb_class->foo = x_grandchild_foo;
    G_OBJECT_CLASS(klass)->finalize = x_grandchild_finalize;
}

int main(int argc, char* argv[])
{
    XBase* xbase = g_object_new(X_TYPE_BASE, NULL);
    foo(xbase);

    getchar();
    g_object_unref(xbase);
    return 0;
}
