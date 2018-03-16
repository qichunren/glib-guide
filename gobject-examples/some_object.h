#ifndef SOME_OBJECT_H
#define SOME_OBJECT_H

#include <glib-object.h>

G_BEGIN_DECLS

#define SOME_TYPE_OBJECT (some_object_get_type())

G_DECLARE_DERIVABLE_TYPE(SomeObject, some_object, SOME, OBJECT, GObject)

SomeObject *some_object_new(void);

G_END_DECLS

#endif /* SOME_OBJECT_H */
