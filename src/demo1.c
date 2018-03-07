#include <stdio.h>
#include <glib.h>
#include <stdio.h>
#include <string.h>

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

  GString * test_string = g_string_new("abcde");
  printf("test string %s len: %lu, and strlen is %lu\n", test_string->str, test_string->len, strlen(test_string->str));

	GMainLoop * g_lpcon_main_loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(g_lpcon_main_loop);
	return 0;
}
