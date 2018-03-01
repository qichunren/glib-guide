#include "some_object.h"

typedef struct
{
	int number;	
} SomeObjectPrivate;

//G_DEFINE_TYPE (SomeObject, some_object, G_TYPE_OBJECT)
G_DEFINE_TYPE_WITH_PRIVATE (SomeObject, some_object, G_TYPE_OBJECT)

enum {
	PROP_0,
	LAST_PROP
};

static GParamSpec *gParamSpecs [LAST_PROP];

SomeObject *
some_object_new (void)
{
	return g_object_new (SOME_TYPE_OBJECT, NULL);
}

static void
some_object_finalize (GObject *object)
{
	SomeObject *self = (SomeObject *)object;
	SomeObjectPrivate *priv = some_object_get_instance_private (self);
	printf("finalize %p %d\n", priv, priv->number);
	printf("finalize %p\n", self);
	
	G_OBJECT_CLASS (some_object_parent_class)->finalize (object);
	
}

static void
some_object_get_property (GObject    *object,
                          guint       prop_id,
                          GValue     *value,
                          GParamSpec *pspec)
{
	SomeObject *self = SOME_OBJECT (object);

	switch (prop_id)
	  {
	  default:
	    G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
	  }
}

static void
some_object_set_property (GObject      *object,
                          guint         prop_id,
                          const GValue *value,
                          GParamSpec   *pspec)
{
	SomeObject *self = SOME_OBJECT (object);

	switch (prop_id)
	  {
	  default:
	    G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
	  }
}

static void
some_object_class_init (SomeObjectClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->finalize = some_object_finalize;
	object_class->get_property = some_object_get_property;
	object_class->set_property = some_object_set_property;
}

static void
some_object_init (SomeObject *self)
{
	SomeObjectPrivate *priv = some_object_get_instance_private (self);
	priv->number = 10;
}
