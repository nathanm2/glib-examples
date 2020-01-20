
#include <glib.h>

static void variant_type_examples(void)
{
    const GVariantType *gtype_const;
    GVariantType *gtype;
    char *type_string;

    /* Internally a GVariantType is a just a string that has been cast to a
     * 'GVariantType'.  The `G_VARIANT_TYPE` macro performs this cast for you.
     *
     * NOTE: The string must be a well-formed GVarieantType type string.
     */
    gtype_const = G_VARIANT_TYPE("b");

    /* To get the original string back call `g_variant_type_dup_string`: */
    type_string = g_variant_type_dup_string(gtype_const);
    g_assert_cmpstr("b", ==, type_string);
    g_free(type_string);

    /* The `g_variant_type_new` simply makes a copy of this string on the heap:
     *  x: a signed 64 bit integer.
     */
    gtype = g_variant_type_new("x");

    /* And freed by calling `g_variant_type_free`: */
    g_variant_type_free(gtype);

    /* There are serveral functions for checking that a string is a valid type
     * string: */
    g_assert_true(g_variant_type_string_is_valid("b"));

    /* A GVariantType must be a single value.  The following is not a valid
     * GVariantType: */
    g_assert_false(g_variant_type_string_is_valid("ii"));

    /* But a tuple is! */
    g_assert_true(g_variant_type_string_is_valid("(ii)"));
}

void variant_examples(void)
{
    /* Use `g_variant_new` to construct a new GVariant instance: */
    GVariant *gvariant = g_variant_new("(xx)", 42, 43);

    /* Use `g_variant_get_type_string` to get the type of the GVariant:
     *
     * Note: the string is owned by the g_variant.
     */
    g_assert_cmpstr(g_variant_get_type_string(gvariant), ==, "(xx)");

    /* Use `g_variant_get` to pull the values from a GVariant: */
    int64_t a, b;
    g_variant_get(gvariant, "(xx)", &a, &b);
    g_assert_cmpint(a, ==, 42);
    g_assert_cmpint(b, ==, 43);

    g_variant_unref(gvariant);
}

void variant_array(void)
{
    /* A "variant builder" is used to construct a variant array. */
    GVariantBuilder *builder;
    GVariant *value;

    builder = g_variant_builder_new(G_VARIANT_TYPE("as"));
    g_variant_builder_add(builder, "s", "when");
    g_variant_builder_add(builder, "s", "in");
    g_variant_builder_add(builder, "s", "the");
    g_variant_builder_add(builder, "s", "course");
    value = g_variant_new("as", builder);
    g_variant_builder_unref(builder);

    GVariantIter *iter;
    gchar *str;

    g_variant_get(value, "as", &iter);
    while (g_variant_iter_loop(iter, "s", &str))
        g_print("%s\n", str);
    g_variant_iter_free(iter);

    g_variant_unref(value);
}

int main(int argc, char *argv[])
{
    variant_type_examples();
    variant_examples();
    variant_array();
}
