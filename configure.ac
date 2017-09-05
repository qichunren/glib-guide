AC_CONFIG_MACRO_DIR(m4)
AC_INIT([glib-guide], [1.0], [bug-automake@gnu.org])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CONFIG_SRCDIR([Makefile.am])


# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_CC_C_O
AC_PROG_INSTALL
AC_PROG_LN_S
AM_PROG_CC_C_O
AC_PROG_LIBTOOL
AM_PROG_LIBTOOL
CFLAGS="-Wall"


PKG_CHECK_MODULES([GLIB2], [glib-2.0 >= 2.32, gthread-2.0 >= 2.32, \
    gmodule-2.0 >= 2.32, gio-2.0 >= 2.32])
PKG_CHECK_MODULES([GSTREAMER], [gstreamer-1.0 >= 1.2.0, \
    gstreamer-base-1.0 >= 1.2.0, gstreamer-audio-1.0 >= 1.2.0 \
    gstreamer-plugins-base-1.0 >= 1.2.0 gstreamer-rtp-1.0 >= 1.2.0])
PKG_CHECK_MODULES([GTK], [gtk+-3.0 >= 3.18])    
    

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
 Makefile
 src/Makefile
])
AC_OUTPUT
