bin_PROGRAMS=http-server

noinst_HEADERS=app-server.h app-client.h

http_server_CFLAGS=@GLIB2_CFLAGS@ @LIBSOUP_CFLAGS@ -DPREFIXDIR=\"$(prefix)\"
http_server_DEPENDENCIES=@LIBOBJS@
http_server_SOURCES=main.c app-server.c app-client.c
http_server_LDFLAGS=-export-dynamic -no-undefined \
    -export-symbols-regex "^[[^_]].*"
http_server_LDADD=@LIBOBJS@ @GLIB2_LIBS@ @GLIB2_LIBS@ @LIBSOUP_LIBS@
