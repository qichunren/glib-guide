#!/bin/sh

rm -rf autoscan.log

rm -rf autom4te.cache
rm -f configure

rm -rf Makefile
rm -rf Makefile.in

rm -rf src/Makefile
rm -rf src/Makefile.in

find -name "*.o" -exec rm {} \;
