#ifndef APP_CLIENT_H
#define APP_CLIENT_H

#include <libsoup/soup.h>

typedef struct _AppClient
{
    SoupWebsocketConnection * ws_connection;
    gint64 created_at;

} AppClient;

AppClient * app_client_new(SoupWebsocketConnection * conn);

#endif
