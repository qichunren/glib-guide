#include <glib.h>
#include <gio/gio.h>

static gboolean tcp_io_watch_cb(GIOChannel *source,
    GIOCondition condition, gpointer user_data)
    {
		if(condition & G_IO_IN)
		{
			g_message("Got receive data");
			}
		}
static gboolean client_incoming_cb(GSocketService *service,
    GSocketConnection *connection, GObject *source_object,
    gpointer user_data)
{
GSocketAddress *remote_address;
    GInetAddress *remote_inetaddr;
    gchar *remote_addrstr = NULL;
    GSocket *socket;
    int fd;
    GIOChannel *channel;

    socket = g_socket_connection_get_socket(connection);
    if(socket==NULL)
    {
        return TRUE;
    }
    fd = g_socket_get_fd(socket);
    if(fd<=0)
    {
        return TRUE;
    }
    channel = g_io_channel_unix_new(fd);
    if(channel==NULL)
    {
        return TRUE;
    }
    g_object_ref(connection);
    g_io_add_watch(channel, G_IO_IN | G_IO_ERR | G_IO_HUP | G_IO_NVAL, tcp_io_watch_cb, user_data);
    if(remote_addrstr!=NULL)
    {
        g_message("New incoming control connection from %s.", remote_addrstr);
    }
    else
    {
        g_message("New incoming control connection from unknown address!");
    }
}
gboolean lpb_ctrl_init(guint16 port)
{
    GSocketService *service;
    GError *error = NULL;
    guint16 rport;
    GObject *state_instance;

    service = g_socket_service_new();
    if(service==NULL)
    {
        g_warning("Failed to create control socket service!");
        return FALSE;
    }
    if(port==0)
    {
        rport = 8080;
    }
    else
    {
        rport = port;
    }
    if(!g_socket_listener_add_inet_port(G_SOCKET_LISTENER(service), rport, NULL, &error))
    {
        g_warning("Failed to bind control socket service: %s", error->message);
        g_clear_error(&error);
        g_object_unref(service);
        return FALSE;
    }
    g_signal_connect(service, "incoming", G_CALLBACK(client_incoming_cb), NULL);

    g_socket_service_start(service);
    g_message("Start cli server");
    return TRUE;
}
