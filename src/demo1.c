#include <stdio.h>
#include <glib.h>

static gboolean demo_timeout_cb(gpointer user_data)
{
    gint64 now;
    now = g_get_monotonic_time();
	g_message("Now %ld", now);
    return TRUE;
}

int main(void)
{
	printf("Hello world!.\n");
	g_timeout_add_seconds(3, demo_timeout_cb, NULL);
	GMainLoop * g_lpcon_main_loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(g_lpcon_main_loop);
	return 0;
}
