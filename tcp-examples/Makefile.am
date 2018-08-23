bin_PROGRAMS=tcp-server main2

noinst_HEADERS=tcp-server.h

tcp_server_CFLAGS=@GLIB2_CFLAGS@ -DPREFIXDIR=\"$(prefix)\"
tcp_server_DEPENDENCIES=@LIBOBJS@
tcp_server_SOURCES=main.c tcp-server.c
tcp_server_LDFLAGS=-export-dynamic -no-undefined \
    -export-symbols-regex "^[[^_]].*"
tcp_server_LDADD=@LIBOBJS@ @GLIB2_LIBS@

main2_CFLAGS=@GLIB2_CFLAGS@ -DPREFIXDIR=\"$(prefix)\"
main2_DEPENDENCIES=@LIBOBJS@
main2_SOURCES=main2.c
main2_LDFLAGS=-export-dynamic -no-undefined \
    -export-symbols-regex "^[[^_]].*"
main2_LDADD=@LIBOBJS@ @GLIB2_LIBS@
