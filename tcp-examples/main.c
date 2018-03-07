#include <glib.h>
#include <stdlib.h>
#include "tcp-server.h"

static GMainLoop * g_main_loop = NULL;

gint main(gint argc, gchar * argv[])
{
    g_set_prgname ("tcp-server");
    g_set_application_name ("TCPServer");

    g_main_loop = g_main_loop_new(NULL, FALSE);

    g_print("Start ");
    g_print(g_get_application_name());
    g_print(" Application");
    g_print("\n");

    TcpServer * tcp_server = tcp_server_new();
    g_print("New TcpServer created.\n");

    gboolean ret = tcp_server_run(tcp_server);
    if(!ret)
    {
      g_critical("Cannot run tcp server");
      exit(1);
    }

    g_main_loop_run(g_main_loop);
    g_main_loop_unref(g_main_loop);

    return 0;
}
