#include "app-client.h"

#include <libsoup/soup.h>

AppClient * app_client_new(SoupWebsocketConnection * conn)
{
    AppClient * client = g_new0(AppClient, 1);
    client->created_at = g_get_monotonic_time();
    client->ws_connection = g_object_ref(conn);

    return client;
}
