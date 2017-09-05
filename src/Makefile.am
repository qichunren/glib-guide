bin_PROGRAMS=demo1 player_demo

noinst_HEADERS= 

demo1_SOURCES=demo1.c

player_demo_CFLAGS=@GLIB2_CFLAGS@ @GSTREAMER_CFLAGS@ @GTK_CFLAGS@
player_demo_DEPENDENCIES=@LIBOBJS@
player_demo_SOURCES=player_demo.c cli_server.c
player_demo_LDFLAGS=-export-dynamic -no-undefined \
    -export-symbols-regex "^[[^_]].*"
player_demo_LDADD=@LIBOBJS@ @GLIB2_LIBS@ @GSTREAMER_LIBS@ @GTK_LIBS@
