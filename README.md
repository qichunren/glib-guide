# Glib guide


## Build steps

	./clean.sh
	./autogen.sh
	mkdir -p build && cd build
	../configure
	make
	./src/demo1

## Debug

Run `export G_MESSAGES_DEBUG=all` to make `g_debug` function output message.

