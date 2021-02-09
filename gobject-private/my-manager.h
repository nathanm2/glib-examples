#pragma once

#include <glib-object.h>

#define MY_TYPE_MANAGER (my_manager_get_type())
G_DECLARE_FINAL_TYPE(MyManager, my_manager, MY, MANAGER, GObject)

struct _MyManager {
    GObject parent_instance;
    char *name;
};

MyManager *my_manager_new(const char *name, int age);

int my_manager_get_age(MyManager *self);
