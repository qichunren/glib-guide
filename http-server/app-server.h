#ifndef APP_SERVER_H
#define APP_SERVER_H

#include <libsoup/soup.h>

typedef struct _AppServer
{
    SoupServer *server;
    GHashTable *clients;

} AppServer;

AppServer * app_server_new();

#endif
