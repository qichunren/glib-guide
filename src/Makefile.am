bin_PROGRAMS=demo1

demo1_CFLAGS=@GLIB2_CFLAGS@ -DPREFIXDIR=\"$(prefix)\"
demo1_DEPENDENCIES=@LIBOBJS@
demo1_SOURCES=demo1.c
demo1_LDFLAGS=-export-dynamic -no-undefined \
    -export-symbols-regex "^[[^_]].*"
demo1_LDADD=@LIBOBJS@ @GLIB2_LIBS@
