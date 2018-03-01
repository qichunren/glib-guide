bin_PROGRAMS=some-object

noinst_HEADERS=some_object.h

some_object_CFLAGS=@GLIB2_CFLAGS@ -DPREFIXDIR=\"$(prefix)\"
some_object_DEPENDENCIES=@LIBOBJS@
some_object_SOURCES=main.c some_object.c
some_object_LDFLAGS=-export-dynamic -no-undefined \
    -export-symbols-regex "^[[^_]].*"
some_object_LDADD=@LIBOBJS@ @GLIB2_LIBS@
