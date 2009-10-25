# Champlain plugin
plugindir = $(libdir)/eog/plugins

INCLUDES = \
	-I$(top_srcdir) 				\
	$(EOG_CFLAGS) 					\
	$(CHAMPLAIN_CFLAGS) 		\
	$(WARN_CFLAGS)					\
	-DEOG_LOCALEDIR=\""$(prefix)/$(DATADIRNAME)/locale"\"

plugin_LTLIBRARIES = libchamplain.la

libchamplain_la_SOURCES = \
	eog-champlain-plugin.h				\
	eog-champlain-plugin.c

libchamplain_la_LDFLAGS = \
	$(PLUGIN_LIBTOOL_FLAGS)				\
	$(EOG_LIBS) \
	$(CHAMPLAIN_LIBS)

# Plugin Info

plugin_in_files = champlain.eog-plugin.desktop.in

%.eog-plugin: %.eog-plugin.desktop.in $(INTLTOOL_MERGE) $(wildcard $(top_srcdir)/po/*po) ; $(AM_V_GEN)$(INTLTOOL_MERGE) $(top_srcdir)/po $< $@ -d -u -c $(top_builddir)/po/.intltool-merge-cache

plugin_DATA = $(plugin_in_files:.eog-plugin.desktop.in=.eog-plugin)

EXTRA_DIST = $(plugin_in_files)

CLEANFILES = $(plugin_DATA)
DISTCLEANFILES = $(plugin_DATA)

-include $(top_srcdir)/git.mk
