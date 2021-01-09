/* Dead Man Switch
 *
 * Reads characters from standard in until nothing has been entered for a
 * specified number of seconds and then exits.
 */
#include <glib.h>
#include <stdbool.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <locale.h>

static gint duration = 3;

static GOptionEntry entries[] = {
    {"time", 't', 0, G_OPTION_ARG_INT, &duration, "Amount of time to wait."},
    {0},
};

typedef struct {
    GMainLoop *loop;
    guint duration;
    guint char_time;
    guint timeout_id;
} DeadManCtx;

static gboolean timeout_handler(void * data)
{
    DeadManCtx *ctx = (DeadManCtx *)data;
    guint cur_time = g_get_monotonic_time() / 1000;
    guint exp_time = ctx->char_time + ctx->duration;

    printf("time=%u exp=%u\n", cur_time, exp_time);
    if (exp_time > cur_time) {
        ctx->timeout_id = g_timeout_add(exp_time - cur_time, timeout_handler,
                                        data);
    } else {
        printf("Timeout expired.\n");
        g_main_loop_quit(ctx->loop);
    }
    return FALSE;
}


static void toggle_timeout(DeadManCtx *ctx)
{
    if (ctx->timeout_id) {
        g_source_remove(ctx->timeout_id);
        printf("Timeout stopped\n");
        ctx->timeout_id = 0;
    } else {
        ctx->timeout_id = g_timeout_add(ctx->duration, timeout_handler, ctx);
        printf("Timeout (re)started\n");
    }
}

static bool process_unichar(gunichar in, DeadManCtx* ctx)
{
    gchar tmp[7] = {0}; /* UTF8 character (6) + NULL terminator (1) */
    g_unichar_to_utf8(in, tmp);

    if (tmp[0] == 'q') {
        printf("Quit\n");
        g_main_loop_quit(ctx->loop);
        return false;
    }

    ctx->char_time = g_get_monotonic_time() / 1000; /* us => ms */
    printf("%s time=%u\n", tmp, ctx->char_time);

    if (tmp[0] == 's') {
        toggle_timeout(ctx);
    }
    return true;
}

/* Invoked when something can be read on standard in: */
static gboolean stdin_cb(GIOChannel *gio, GIOCondition cond, gpointer data)
{
    gunichar in;
    GError *error = NULL;
    DeadManCtx *ctx = (DeadManCtx *)data;

    switch (g_io_channel_read_unichar(gio, &in, &error)) {

    case G_IO_STATUS_NORMAL:
        return process_unichar(in, ctx) ? TRUE : FALSE;

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

static bool setup_stdin(DeadManCtx *ctx)
{
    GIOChannel *stdin_ch;
    GIOStatus status;
    GError *error = NULL;

    enter_raw_mode();

    stdin_ch = g_io_channel_unix_new(STDIN_FILENO);
    if (!stdin_ch) {
        g_printerr("Cannot create a GIO channel for standard in.\n");
        return false;
    }

    status = g_io_channel_set_flags(stdin_ch, G_IO_FLAG_NONBLOCK, &error);
    if (status == G_IO_STATUS_ERROR) {
        g_printerr("Cannot set non-blocking: %s", error->message);
        return false;
    }

    if (!g_io_add_watch(stdin_ch, G_IO_IN, stdin_cb, ctx)) {
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
    GMainLoop *loop;
    DeadManCtx ctx;

    setlocale(LC_CTYPE, "");

    memset(&ctx, 0, sizeof(ctx));

    /* Process CLI arguments: */
    oc = g_option_context_new(NULL);
    g_option_context_add_main_entries(oc, entries, NULL);
    if (!g_option_context_parse(oc, &argc, &argv, &error)) {
        g_printerr("%s\n", error->message);
        exit(1);
    }
    g_option_context_free(oc);

    /* Create the event loop: */
    ctx.loop = loop = g_main_loop_new(NULL, false);

    /* Setup standard in: */
    if (!setup_stdin(&ctx))
        goto cleanup;

    ctx.duration = duration * 1000;

    /* Let's roll! */
    g_main_loop_run(loop);

cleanup:
    g_main_loop_unref(loop);

    return 0;
}
