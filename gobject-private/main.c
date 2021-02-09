#include <stdio.h>

#include "my-manager.h"

int main(int argc, char *argv[])
{
    MyManager *m1 = my_manager_new("sean", 43);
    MyManager *m2 = my_manager_new("nate", 40);

    printf("%s %d\n", m1->name, my_manager_get_age(m1));
    printf("%s %d\n", m2->name, my_manager_get_age(m2));

    g_object_unref(m1);
    g_object_unref(m2);

    return 0;
}
