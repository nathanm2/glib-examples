/* Dead Man Switch
 *
 * Program exits if user input isn't recieved within X seconds.
 */
#include <glib.h>
#include <stdbool.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

static gint duration = 3;

static GOptionEntry entries[] = {
    {"time", 't', 0, G_OPTION_ARG_INT, &duration, "Amount of time to wait."},
    {0},
};

static bool stdin_handler(char in)
{
    switch (in) {
    case 'q':
        printf("Quit\n");
        g_main_loop_quit(NULL);
        return false;
    default:
        printf("%c\n", in);
    }
    return true;
}

/* Invoked when something can be read on standard in: */
static gboolean stdin_cb(GIOChannel *gio, GIOCondition cond, gpointer data)
{
    gchar in;
    GError *error = NULL;

    switch (g_io_channel_read_chars(gio, &in, 1, NULL, &error)) {

    case G_IO_STATUS_NORMAL:
        return stdin_handler(in) ? TRUE : FALSE;

    case G_IO_STATUS_ERROR:
        g_printerr("IO error: %s\n", error->message);
        g_error_free(error);
        return FALSE;

    case G_IO_STATUS_AGAIN:
        return TRUE;

    default:
        g_return_val_if_reached(FALSE);
        break;
    }
}

/* Terminal settings cache: */
struct termios orig_termios;

static void disable_raw_mode()
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

static void enter_raw_mode()
{
    struct termios term;

    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disable_raw_mode);

    term = orig_termios;

    term.c_lflag &= ~(ECHO | ICANON);
    term.c_cc[VMIN] = 1;
    term.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &term);
}

static bool setup_stdin(void)
{
    GIOChannel *stdin_ch;

    enter_raw_mode();

    stdin_ch = g_io_channel_unix_new(STDIN_FILENO);
    if (!stdin_ch) {
        g_printerr("Cannot create a GIO channel for standard in.\n");
        return false;
    }

    if (!g_io_add_watch(stdin_ch, G_IO_IN, stdin_cb, NULL)) {
        g_printerr("Cannot add watch on standard in channel!\n");
        return false;
    }

    g_io_channel_unref(stdin_ch);

    return true;
}

int main(int argc, char *argv[])
{
    GOptionContext *oc;
    GError *error = NULL;

    /* Process CLI arguments: */
    oc = g_option_context_new(NULL);
    g_option_context_add_main_entries(oc, entries, NULL);
    if (!g_option_context_parse(oc, &argc, &argv, &error)) {
        g_printerr("%s\n", error->message);
        exit(1);
    }
    g_option_context_free(oc);

    /* Create the event loop:
        - Use the default context.
        - Don't set to running.
    */
    GMainLoop *loop = g_main_loop_new(NULL, false);

    /* Listen on standard input */
    if (!setup_stdin())
        goto cleanup;

    g_main_loop_run(loop);

cleanup:
    g_main_loop_unref(loop);

    return 0;
}
