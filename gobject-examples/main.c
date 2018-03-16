#include "some_object.h"
#include "example-person.h"
#include "example-worker.h"
#include <glib.h>

gint main(gint argc, gchar *argv[])
{
    g_set_prgname("GObject Example");
    g_set_application_name("GObjectExample");

    GMainLoop * main_loop = g_main_loop_new(NULL, FALSE);

    g_print("Start ");
    g_print(g_get_application_name());
    g_print(" Application\n");

    SomeObject *some = some_object_new();
    g_print("New Object Created\n");
    g_object_unref(some);

    ExamplePerson * person = example_person_new("qcr", 31, 1);
    example_person_introduce_self(person);
    example_person_say_hello(person, "Hey, how are you?");
    g_object_unref(person);

    ExampleWorker * worker = example_worker_new("qcr-worker", 30, 1, "Software engineer");
    example_worker_introduce_self(worker);
    g_object_unref(worker);

    g_main_loop_run(main_loop);
    g_main_loop_unref(main_loop);

    return 0;
}
