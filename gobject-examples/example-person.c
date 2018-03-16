#include "example-person.h"


typedef enum
{
    FEMALE,
    MALE,
    SEX_LAST
} SexFlag;

struct _ExamplePersonClass
{
    GObjectClass parent;
};

typedef struct
{
    gchar * name;
    guint age;
    SexFlag sex;

} ExamplePersonPrivate;

enum
{
    PROP_0,
    PROP_NAME,
    PROP_AGE,
    PROP_SEX,
    N_PROPERTIES
};

static GParamSpec * properties[N_PROPERTIES] = { NULL, };

G_DEFINE_TYPE_WITH_PRIVATE(ExamplePerson, example_person, G_TYPE_OBJECT)


static void example_person_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec * pspec)
{
    ExamplePerson * self = EXAMPLE_PERSON(object);
    ExamplePersonPrivate * priv = example_person_get_instance_private(self);

    switch(prop_id)
    {
        case PROP_NAME:
            g_value_set_string(value, priv->name);
            break;
        case PROP_AGE:
            g_value_set_uint(value, priv->age);
            break;
        case PROP_SEX:
            g_value_set_uint(value, priv->sex);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}

static void example_person_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec * pspec)
{
    ExamplePerson *self = EXAMPLE_PERSON(object);
    ExamplePersonPrivate * priv = example_person_get_instance_private(self);

    switch(prop_id)
    {
        case PROP_NAME:
            priv->name = g_strdup(g_value_get_string(value));
            break;
        case PROP_AGE:
            priv->age = g_value_get_uint(value);
            break;
        case PROP_SEX:
            priv->sex = (SexFlag)g_value_get_uint(value);
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}

static void example_person_finalize(GObject * object)
{
    ExamplePerson *self = EXAMPLE_PERSON(object);
    ExamplePersonPrivate * priv = example_person_get_instance_private(self);

    g_free(priv->name);

    G_OBJECT_CLASS (example_person_parent_class)->finalize(object);
    g_debug("example_person_finalize");
}

static void example_person_class_init(ExamplePersonClass *klass)
{
    GObjectClass * object_class = G_OBJECT_CLASS(klass);

    properties[PROP_NAME] = g_param_spec_string("name", "Name", "Name", "", G_PARAM_READWRITE);
    properties[PROP_AGE] = g_param_spec_uint("age", "Age", "Age", 0, G_MAXINT, 0, G_PARAM_READWRITE);
    properties[PROP_SEX] = g_param_spec_uint ("sex", "Sex", "Sex", FEMALE, MALE, MALE, G_PARAM_READWRITE);

    object_class->finalize = example_person_finalize;
    object_class->get_property = example_person_get_property;
    object_class->set_property = example_person_set_property;

    g_object_class_install_properties(object_class, N_PROPERTIES, properties);
}

static void example_person_init(ExamplePerson *self)
{

}

ExamplePerson * example_person_new(gchar * name, int age, int sex)
{
    ExamplePerson * person = g_object_new(EXAMPLE_TYPE_PERSON, "name", name, "age", age, "sex", sex, NULL);
    return person;
}

void example_person_introduce_self(ExamplePerson * person)
{
    g_return_if_fail(person != NULL);

    ExamplePersonPrivate * priv = example_person_get_instance_private(person);

    g_message("Person introduce self: I am %s, %d years old, %d", priv->name, priv->age, priv->sex);
}

void example_person_say_hello(ExamplePerson * person, gchar * message)
{
    g_return_if_fail(person != NULL);

    g_message("Person say hello: %s", message);
}
