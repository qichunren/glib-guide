bin_PROGRAMS=demo1 demo2

demo1_CFLAGS=@GLIB2_CFLAGS@ -DPREFIXDIR=\"$(prefix)\"
demo1_DEPENDENCIES=@LIBOBJS@
demo1_SOURCES=demo1.c
demo1_LDFLAGS=-export-dynamic -no-undefined \
    -export-symbols-regex "^[[^_]].*"
demo1_LDADD=@LIBOBJS@ @GLIB2_LIBS@


demo2_CFLAGS=@GLIB2_CFLAGS@ @GSTREAMER_CFLAGS@ -DPREFIXDIR=\"$(prefix)\"
demo2_DEPENDENCIES=@LIBOBJS@
demo2_SOURCES=demo2.c
demo2_LDFLAGS=-export-dynamic -no-undefined \
    -export-symbols-regex "^[[^_]].*"
demo2_LDADD=@LIBOBJS@ @GLIB2_LIBS@ @GSTREAMER_LIBS@
