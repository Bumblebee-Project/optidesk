# -*- Makefile -*-

GITVERSION = $(shell "$(top_srcdir)/version.sh" "$(top_srcdir)")

AM_CPPFLAGS = ${regular_CPPFLAGS}
AM_CFLAGS = ${regular_CFLAGS} \
		${glib_CFLAGS} \
		${gio_CFLAGS}

bin_PROGRAMS = bin/optidesk

bin_optidesk_SOURCES = optidesk.c
bin_optidesk_LDADD = ${glib_LIBS} ${gio_LIBS}
