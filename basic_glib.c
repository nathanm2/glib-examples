
#include <stdio.h>
#include <glib.h>

typedef struct foo {
    int a;
    int b;
} foo_t;

int main(int argc, char* argv[])
{
    foo_t* foos = g_new(foo_t, 4);
    g_free(foos);
    printf("It works!\n");
    return 0;
}
