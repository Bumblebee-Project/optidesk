#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([optidesk],[0.1])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_FILES([Makefile])

AM_INIT_AUTOMAKE([1.10.2 -Wall foreign subdir-objects
	tar-pax])

# Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O

regular_CPPFLAGS="-D_FILE_OFFSET_BITS=64 -D_REENTRANT"
regular_CFLAGS="-Wall -Waggregate-return \
	-Wmissing-declarations -Wmissing-prototypes -Wredundant-decls \
	-Wshadow -Wstrict-prototypes -Winline -Wformat=2 -pipe"
AC_SUBST([regular_CPPFLAGS])
AC_SUBST([regular_CFLAGS])

# Checks for header files.
PKG_CHECK_MODULES([glib], [glib-2.0])
AC_OUTPUT
