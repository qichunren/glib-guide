## GObject 使用笔记
GObject

实例化一个GObject（或者子类）是通过调用 `g_object_new(MODULE_TYPE_NAME, NULL);` 函数来生成对象的。 `g_object_new` 然后会先后调用 `module_name_class_init`函数和 `module_app_init` 函数。其中 `module_name_class_init` 只会在第一次创建实例时被调用， 在后续创建实例的时候 `module_app_init` 会被调用。


`module_name_class_init` 方法中用来指定get_property、set_property、dispose、finalize各种方法、创建信号（SIGNAL）等工作。

`module_name_init` 方法中用来初始化类 `ModuleName` 的成员和私有 `ModuleNamePrivate` 成员。
