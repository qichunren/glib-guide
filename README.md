# Glib guide

## Install dependency

    sudo apt-get install libsoup2.4-dev

## Build steps

    ./clean.sh
    ./autogen.sh
    mkdir -p build && cd build
    ../configure
    make
    ./src/demo1

## Debug

Run `export G_MESSAGES_DEBUG=all` to make `g_debug` function output message.


## 参考资源

* [GObject Reference Manual](https://developer.gnome.org/gobject/stable/index.html)
* [How to define and implement a new GObject](https://developer.gnome.org/gobject/stable/howto-gobject.html)
* [Boilerplate code](https://developer.gnome.org/gobject/stable/howto-gobject-code.html)
* [如何定义和实现新的GObject之代码实现](https://blog.csdn.net/lp525110627/article/details/71731595)
* [如何定义和实现新的GObject之对象构造和析构](https://blog.csdn.net/lp525110627/article/details/71743173)
* [Manage C data using the GLib collections](https://www.ibm.com/developerworks/linux/tutorials/l-glib/)
* [使用 GObject 库模拟类的数据封装形式](http://garfileo.is-programmer.com/2011/2/27/the-analog-of-classed-type-based-gobject.24798.html)
