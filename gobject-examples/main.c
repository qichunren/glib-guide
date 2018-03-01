#include <glib.h>
#include "some_object.h"

gint
main (gint   argc,
      gchar *argv[])
{
	g_set_prgname ("SomeObject");
	g_set_application_name ("SomeObject");

  	g_print("Start ");
  	g_print(g_get_application_name());
  	g_print(" Application");
  	g_print("\n");

  	SomeObject *some = some_object_new();
  	g_print("New Object Created\n");
  	g_object_unref(some);

	return 0;
}
