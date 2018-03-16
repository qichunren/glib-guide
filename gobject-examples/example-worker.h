#ifndef EXAMPLE_WORKER_H
#define EXAMPLE_WORKER_H

#include "example-person.h"

G_BEGIN_DECLS

struct _ExampleWorkerClass
{
    ExamplePersonClass parent;
};

#define EXAMPLE_TYPE_WORKER (example_worker_get_type())

G_DECLARE_DERIVABLE_TYPE(ExampleWorker, example_worker, EXAMPLE, WORKER, ExamplePerson)

ExampleWorker * example_worker_new(gchar * name, int age, int sex, gchar * title);

void example_worker_introduce_self(ExampleWorker * person);

G_END_DECLS

#endif
