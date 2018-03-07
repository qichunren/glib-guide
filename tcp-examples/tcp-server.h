#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <glib-object.h>

G_BEGIN_DECLS

#define TCP_TYPE_SERVER (tcp_server_get_type())

G_DECLARE_DERIVABLE_TYPE(TcpServer, tcp_server, TCP, SERVER, GObject)

struct _TcpServerClass
{
	GObjectClass parent;
};

TcpServer * tcp_server_new(void);
gboolean tcp_server_run(TcpServer *);
gboolean tcp_server_send(TcpServer *, GString * data);

G_END_DECLS

#endif /* TCP_SERVER_H */
