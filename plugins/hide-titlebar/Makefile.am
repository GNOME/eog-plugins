# HideTitlebar plugin
plugindir = $(libdir)/eog/plugins

INCLUDES = \
	-I$(top_srcdir)/src 				\
	$(EOG_CFLAGS) 					\
	$(WARN_CFLAGS)					\
	-DEOG_LOCALEDIR=\""$(prefix)/$(DATADIRNAME)/locale"\"

plugin_LTLIBRARIES = libhide-titlebar.la

libhide_titlebar_la_SOURCES = \
	eog-hide-titlebar-plugin.h			\
	eog-hide-titlebar-plugin.c

libhide_titlebar_la_LDFLAGS = \
	-avoid-version -module

libhide_titlebar_la_LIBADD = $(EOG_LIBS)

# Plugin Info

plugin_in_files = hide-titlebar.plugin.desktop.in

%.plugin: %.plugin.desktop.in $(INTLTOOL_MERGE) $(wildcard $(top_srcdir)/po/*po) ; $(AM_V_GEN)$(INTLTOOL_MERGE) $(top_srcdir)/po $< $@ -d -u -c $(top_builddir)/po/.intltool-merge-cache

plugin_DATA = $(plugin_in_files:.plugin.desktop.in=.plugin)

EXTRA_DIST = $(plugin_in_files)

CLEANFILES = $(plugin_DATA)
DISTCLEANFILES = $(plugin_DATA)

-include $(top_srcdir)/git.mk
