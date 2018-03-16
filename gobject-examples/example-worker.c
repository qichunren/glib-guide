#include "example-worker.h"

typedef struct
{
    gchar * title;

} ExampleWorkerPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(ExampleWorker, example_worker, EXAMPLE_TYPE_PERSON)

static void example_worker_finalize(GObject * object)
{
    ExampleWorker *self = EXAMPLE_WORKER(object);
    ExampleWorkerPrivate * priv = example_worker_get_instance_private(self);

    g_free(priv->title);

    G_OBJECT_CLASS (example_worker_parent_class)->finalize(object);
    g_debug("example_worker_finalize");
}

static void example_worker_class_init(ExampleWorkerClass *klass)
{
    GObjectClass * object_class = G_OBJECT_CLASS(klass);
    object_class->finalize = example_worker_finalize;

}

static void example_worker_init(ExampleWorker *self)
{
    g_message("example_worker_init");
    ExampleWorkerPrivate * priv = example_worker_get_instance_private(self);
    if(priv->title != NULL)
    {
        g_warning("example worker instance title property is not null when init");
    }
}

ExampleWorker * example_worker_new(gchar * name, int age, int sex, gchar * title)
{
    ExampleWorker * person = g_object_new(EXAMPLE_TYPE_WORKER, "name", name, "age", age, "sex", sex, NULL);
    return person;
}

void example_worker_introduce_self(ExampleWorker * person)
{
    g_return_if_fail(person != NULL);

    example_person_introduce_self(person);

    ExampleWorkerPrivate * priv = example_worker_get_instance_private(person);

    g_message("Worker introduce self: my title is %s", priv->title);
}
