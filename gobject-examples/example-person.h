#ifndef EXAMPLE_PERSON_H
#define EXAMPLE_PERSON_H

#include <glib-object.h>

G_BEGIN_DECLS

struct _ExamplePersonClass
{
    GObjectClass parent;
};

#define EXAMPLE_TYPE_PERSON (example_person_get_type())

G_DECLARE_DERIVABLE_TYPE(ExamplePerson, example_person, EXAMPLE, PERSON, GObject)

ExamplePerson * example_person_new(gchar * name, int age, int sex);

void example_person_introduce_self(ExamplePerson * person);

void example_person_say_hello(ExamplePerson * person, gchar * message);

G_END_DECLS
#endif
