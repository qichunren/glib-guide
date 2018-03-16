bin_PROGRAMS=gobject-example

noinst_HEADERS=some_object.h example-person.h example-worker.h example-boss.h

gobject_example_CFLAGS=@GLIB2_CFLAGS@ -DPREFIXDIR=\"$(prefix)\"
gobject_example_DEPENDENCIES=@LIBOBJS@
gobject_example_SOURCES=main.c some_object.c example-person.c example-worker.c example-boss.c
gobject_example_LDFLAGS=-export-dynamic -no-undefined \
    -export-symbols-regex "^[[^_]].*"
gobject_example_LDADD=@LIBOBJS@ @GLIB2_LIBS@
