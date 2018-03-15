#include "app-server.h"
#include "app-client.h"

#include <libsoup/soup.h>

static void app_client_free(AppClient * client)
{
    if(client == NULL)
    {
        return;
    }
    g_object_unref(client->ws_connection);
    g_free(client);
}


AppServer * app_server_new()
{
    AppServer * app_server = g_new0(AppServer, 1);
    app_server->server = soup_server_new(SOUP_SERVER_SERVER_HEADER, "simple-httpd ", NULL);
    app_server->clients = g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, (GDestroyNotify)app_client_free);

    return app_server;
}
