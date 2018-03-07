#include "tcp-server.h"
#include <gio/gio.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <glib/gprintf.h>

typedef struct _ConnectionData
{
    GSocketConnection * connection;
    GIOChannel * channel;
    int fd;
    GString * read_buffer;
    GString * write_remaining;
    GAsyncQueue * write_queue;
    guint io_watch_id;
} ConnectionData;

typedef struct
{
  uint server_port;
  GSocketService * socket_service;
  GHashTable * connection_table;
    
} TcpServerPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(TcpServer, tcp_server, G_TYPE_OBJECT)

enum {
    PROP_NONE,
    PROP_PORT,
    LAST_PROP
};

static gboolean tcp_server_socket_io_watch_cb(GIOChannel * source, GIOCondition condition, gpointer user_data);

static GParamSpec * gParamSpecs[LAST_PROP];

TcpServer * tcp_server_new(void)
{
    return g_object_new(TCP_TYPE_SERVER, "port", 5555, NULL);
}

static void tcp_server_finalize(GObject * object)
{
    TcpServer * self = (TcpServer *)object;
    TcpServerPrivate * priv = tcp_server_get_instance_private(self);

    g_object_unref(priv->socket_service);
    priv->socket_service = NULL;

    g_hash_table_unref(priv->connection_table);
    priv->connection_table = NULL;
	
    G_OBJECT_CLASS (tcp_server_parent_class)->finalize(object);
}

static void tcp_server_get_property(GObject * object, guint prop_id, GValue * value, GParamSpec * pspec)
{
    TcpServer * self = TCP_SERVER(object);
    TcpServerPrivate * priv = tcp_server_get_instance_private(self);

    switch(prop_id)
    {
        case PROP_PORT:
              g_value_set_uint(value, priv->server_port);
              g_print ("server port: %u\n", priv->server_port);
              break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}

static void tcp_server_set_property(GObject * object, guint prop_id, GValue * value, GParamSpec * pspec)
{
    TcpServer * self = TCP_SERVER(object);
    TcpServerPrivate * priv = tcp_server_get_instance_private(self);

    switch(prop_id)
    {
        case PROP_PORT:
            priv->server_port = g_value_get_uint(value);
            g_print ("set server port: %u\n", priv->server_port);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void tcp_server_class_init(TcpServerClass * klass)
{
    GObjectClass * object_class = G_OBJECT_CLASS(klass);

    object_class->finalize = tcp_server_finalize;
    object_class->get_property = tcp_server_get_property;
    object_class->set_property = tcp_server_set_property;

    gParamSpecs[PROP_PORT] = g_param_spec_uint("port", "Socket port", "Tcp server port.",
                                                  1000  /* minimum value */,
                                                  65535 /* maximum value */,
                                                  3456  /* default value */,
                                                  G_PARAM_READWRITE);

    g_object_class_install_properties(object_class, LAST_PROP, gParamSpecs);
}

static void connection_data_destroy(ConnectionData * data)
{
    if(data == NULL)
    {
        return;
    }
    if(data->read_buffer != NULL)
    {
        g_string_free(data->read_buffer, TRUE);
    }
    if(data->write_remaining != NULL)
    {
        g_string_free(data->write_remaining, TRUE);
    }
    if(data->write_queue != NULL)
    {
        g_async_queue_unref(data->write_queue);
    }
    if(data->channel != NULL)
    {
        g_io_channel_unref(data->channel);
    }
    if(data->connection != NULL)
    {
        g_object_unref(data->connection);
    }
    g_free(data);
}

static gboolean tcp_server_socket_io_watch_cb(GIOChannel * source, GIOCondition condition, gpointer user_data) {
    TcpServer * ctrl_data = (TcpServer *)user_data;
    TcpServerPrivate * priv = tcp_server_get_instance_private(ctrl_data);
    ConnectionData * connection_data;
    gchar buffer[16385];
    ssize_t recv_num, total_send = 0, send_num, send_remaining;
    struct sockaddr_in cliaddr;
    socklen_t addr_len;
    gboolean ret = TRUE;
    GString * write_data;
    gboolean writable = TRUE;
    gboolean wouldblock = FALSE;
    gsize segsize;

    addr_len = sizeof(struct sockaddr_in);
    connection_data = g_hash_table_lookup(priv->connection_table, source);
      
    printf("invoke lps_ctrl_tcp_io_watch_cb\n");
      
    if(connection_data == NULL)
    {
        return FALSE;
    }

  if(condition & G_IO_IN)
  {
		
      int recv_times = 1;
      while( (recv_num = recvfrom(connection_data->fd, buffer, 10, 0, (struct sockaddr *)&cliaddr, &addr_len)) > 0)
      {
        printf("# %d recv data: %s , recv_num %ld\n", recv_times, buffer, recv_num);
        recv_times++;
        if(connection_data->read_buffer->len < 2097152)
        {
            g_string_append_len(connection_data->read_buffer, buffer, recv_num);
        }
        g_strdelimit(connection_data->read_buffer->str, "\n", '\0');
        while(connection_data->read_buffer->len > 0 && (segsize = strlen(connection_data->read_buffer->str)) != connection_data->read_buffer->len)
        {
            // ret = !lps_ctrl_data_parse(ctrl_data, connection_data, connection_data->read_buffer, segsize);
            printf("connection_data->read_buffer->str: %s\n", connection_data->read_buffer->str);
            g_string_erase(connection_data->read_buffer, 0, segsize + 1);
        }
      }
      if(recv_num == 0)
      {
          g_hash_table_remove(priv->connection_table, source);
          g_message("A control client disconnected.");
          return FALSE;
      }
      else
      {
        if(errno==EAGAIN)
        {
          //Blocked
          // printf("blocked.\n");
        }
      }
      return ret;
  }
  else if(condition & G_IO_OUT)
  {
        writable = TRUE;
        wouldblock = FALSE;
        if(connection_data->write_remaining != NULL)
        {
            write_data = connection_data->write_remaining;
            while(total_send < write_data->len+1)
            {
                send_remaining = write_data->len + 1 - total_send;
                send_num = send(connection_data->fd, write_data->str+total_send, send_remaining > 16384 ? 16384 : send_remaining, 0);
                if(send_num > 0)
                {
                    total_send += send_num;
                }
                else
                {
                    if(errno == EWOULDBLOCK)
                    {
                        g_string_erase(write_data, 0, total_send);
                        wouldblock = TRUE;
                    }
                    writable = FALSE;
                    break;
                }
            }
            if(!wouldblock)
            {
                connection_data->write_remaining = NULL;
                g_string_free(write_data, TRUE);
            }
        }
        if(writable)
        {
            write_data = g_async_queue_try_pop(connection_data->write_queue);
        }
        else
        {
            write_data = NULL;
        }
        if(write_data!=NULL)
        {
            wouldblock = FALSE;
            while(total_send < write_data->len+1)
            {
                send_remaining = write_data->len + 1 - total_send;
                send_num = send(connection_data->fd, write_data->str+total_send, send_remaining > 16384 ? 16384 : send_remaining, 0);
                if(send_num > 0)
                {
                    total_send += send_num;
                }
                else
                {
                    if(errno == EWOULDBLOCK)
                    {
                        g_string_erase(write_data, 0, total_send);
                        connection_data->write_remaining = write_data;
                        wouldblock = TRUE;
                    }
                    break;
                }
            }
            if(!wouldblock)
            {
                g_string_free(write_data, TRUE);
            }
            if(g_async_queue_length(connection_data->write_queue) > 0)
            {
                wouldblock = TRUE;
            }
            if(!wouldblock)
            {
                if(connection_data->io_watch_id>0)
                {
                    g_source_remove(connection_data->io_watch_id);
                }
                connection_data->io_watch_id = g_io_add_watch(
                    connection_data->channel, G_IO_IN | G_IO_ERR | G_IO_HUP |
                    G_IO_NVAL, tcp_server_socket_io_watch_cb, user_data);
            }
        }
        ret = wouldblock;
        return ret;
    }
    else if(condition & (G_IO_ERR | G_IO_HUP | G_IO_NVAL))
    {
        g_hash_table_remove(priv->connection_table, source);
        g_message("A control client disconnected.");
        return FALSE;
    }

    return FALSE;
}

static void tcp_server_init(TcpServer * self)
{
    TcpServerPrivate * priv = tcp_server_get_instance_private(self);
    priv->connection_table = g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, (GDestroyNotify)connection_data_destroy);
    if(priv->connection_table == NULL)
    {
      g_warning("Failed to create connection table!");
    }
    
    priv->socket_service = g_socket_service_new();
    if(priv->socket_service == NULL)
    {
      g_warning("Failed to create socket service!");
    }
}

static void lps_ctrl_string_free(GString * str)
{
    g_string_free(str, TRUE);
}

static ConnectionData * connection_data_new(GSocketConnection * connection)
{
    ConnectionData * data = g_new0(ConnectionData, 1);
    data->read_buffer = g_string_new("");
    data->write_queue = g_async_queue_new_full((GDestroyNotify)lps_ctrl_string_free);
    data->connection = g_object_ref(connection);
    return data;
}

static gboolean client_connected_cb(GSocketService * service, GSocketConnection * connection, GObject * source_object, gpointer user_data)
{
    ConnectionData * connection_data;
    TcpServer * ctrl_data = (TcpServer *)user_data;
    TcpServerPrivate * priv = tcp_server_get_instance_private (ctrl_data);
    GSocketAddress * remote_address;
    GInetAddress * remote_inetaddr;
    gchar * remote_addrstr = NULL;
    GSocket * socket;
    int fd;
    GIOChannel *channel;

    socket = g_socket_connection_get_socket(connection);
    if(socket==NULL)
    {
        return TRUE;
    }
    fd = g_socket_get_fd(socket);
    if(fd <= 0)
    {
        return TRUE;
    }
    channel = g_io_channel_unix_new(fd);
    if(channel == NULL)
    {
        return TRUE;
    }
    
    
    if(priv->connection_table == NULL)
    {
		g_printf("Error, connection table is null\n");
	}
    
    connection_data = connection_data_new(connection);
    connection_data->channel = channel;
    connection_data->fd = fd;
    g_hash_table_replace(priv->connection_table, channel, connection_data);
    connection_data->io_watch_id = g_io_add_watch(channel, G_IO_IN | G_IO_ERR | G_IO_HUP | G_IO_NVAL, tcp_server_socket_io_watch_cb, user_data);    

    remote_address = g_socket_connection_get_remote_address(connection, NULL);
    if(remote_address != NULL)
    {
        remote_inetaddr = g_inet_socket_address_get_address(G_INET_SOCKET_ADDRESS(remote_address));
        remote_addrstr = g_inet_address_to_string(remote_inetaddr);
        g_object_unref(remote_address);
    }
    if(remote_addrstr != NULL)
    {
        g_message("New incoming control connection from %s.", remote_addrstr);
    }
    else
    {
        g_message("New incoming control connection from unknown address!");
    }

    return TRUE;
}

gboolean tcp_server_run(TcpServer * self)
{
    GError * error = NULL;
    TcpServerPrivate * priv = tcp_server_get_instance_private (self);
    if(!g_socket_listener_add_inet_port(G_SOCKET_LISTENER(priv->socket_service), priv->server_port, NULL, &error))
    {
        g_warning("Failed to bind control socket service: %s", error->message);
        g_clear_error(&error);
        g_object_unref(priv->socket_service);
        return FALSE;
    }
    g_signal_connect(priv->socket_service, "incoming", G_CALLBACK(client_connected_cb), self);
    g_socket_service_start(priv->socket_service);
    g_printf("Server listen on port: %u\n", priv->server_port);
}
