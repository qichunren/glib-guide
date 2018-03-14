# -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_CONFIG_MACRO_DIR(m4)

m4_define([glibguide_major_version], [1])
m4_define([glibguide_minor_version], [0])
m4_define([glibguide_micro_version], [0])
m4_define([glibguide_version], [glibguide_major_version.glibguide_minor_version.glibguide_micro_version])

AC_INIT([glib-guide], [glibguide_version], [bug-automake@gnu.org])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CONFIG_SRCDIR([Makefile.am])
AM_SILENT_RULES([yes])

GLIBGUIDE_MAJOR_VERSION=glibguide_major_version
GLIBGUIDE_MINOR_VERSION=glibguide_minor_version
GLIBGUIDE_MICRO_VERSION=glibguide_micro_version
GLIBGUIDE_VERSION=glibguide_version
GLIBGUIDE_LIB_VERSION=glibguide_lib_version

AC_DEFINE(GLIBGUIDE_MAJOR_VERSION, [glibguide_major_version],
    [Define to the Glib Guide major version])
AC_DEFINE(GLIBGUIDE_MINOR_VERSION, [glibguide_minor_version],
    [Define to the Glib Guide minor version])
AC_DEFINE(GLIBGUIDE_MICRO_VERSION, [glibguide_micro_version],
    [Define to the Glib Guide micro version])

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

PKG_CHECK_MODULES([GLIB2], [glib-2.0 >= 2.32, gthread-2.0 >= 2.32, gmodule-2.0 >= 2.32, gio-2.0 >= 2.32])
PKG_CHECK_MODULES([GSTREAMER], [gstreamer-1.0 >= 1.2.0, gstreamer-base-1.0 >= 1.2.0, gstreamer-audio-1.0 >= 1.2.0 gstreamer-plugins-base-1.0 >= 1.2.0 gstreamer-rtp-1.0 >= 1.2.0])

PKG_CHECK_MODULES(LIBSOUP, [libsoup-2.4 >= 2.26])
AC_SUBST(LIBSOUP_CFLAGS)
AC_SUBST(LIBSOUP_LIBS)

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
 Makefile
 src/Makefile
 gobject-examples/Makefile
 tcp-examples/Makefile
 http-server/Makefile
])
AC_OUTPUT
