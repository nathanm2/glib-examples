#include "my-manager.h"

typedef struct {
    int age;
} MyManagerPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(MyManager, my_manager, G_TYPE_OBJECT)

void my_manager_init(MyManager *self) {}

void my_manager_finalize(GObject *obj)
{
    MyManager *self = MY_MANAGER(obj);
    g_free(self->name);

    G_OBJECT_CLASS(my_manager_parent_class)->finalize(obj);
}

void my_manager_class_init(MyManagerClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    object_class->finalize = my_manager_finalize;
}

MyManager *my_manager_new(const char *name, int age)
{
    MyManager *self = g_object_new(MY_TYPE_MANAGER, NULL);
    MyManagerPrivate *p = my_manager_get_instance_private(self);
    self->name = g_strdup(name);
    p->age = age;
    return self;
}

int my_manager_get_age(MyManager *self)
{
    MyManagerPrivate *p = my_manager_get_instance_private(self);
    return p->age;
}
