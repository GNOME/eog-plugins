# Postasa plugin
plugindir = $(libdir)/eog/plugins

INCLUDES = \
	-I$(top_srcdir) 				\
	$(EOG_CFLAGS) 					\
	$(WARN_CFLAGS)					\
	$(POSTASA_CFLAGS) 				\
	-DEOG_LOCALEDIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DEOG_PLUGINDIR=\""$(plugindir)"\"

plugin_LTLIBRARIES = libpostasa.la

libpostasa_la_SOURCES = \
	eog-postasa-plugin.h				\
	eog-postasa-plugin.c

libpostasa_la_LDFLAGS = \
	$(PLUGIN_LIBTOOL_FLAGS)				\
	$(EOG_LIBS)

libpostasa_la_LIBADD = $(EOG_LIBS)			\
	$(POSTASA_LIBS)


# Plugin Info

plugin_in_files = postasa.eog-plugin.desktop.in

UI_FILES = postasa-config.xml postasa-uploads.xml
ui_DATA = $(UI_FILES)
uidir = $(plugindir)/postasa

%.eog-plugin: %.eog-plugin.desktop.in $(INTLTOOL_MERGE) $(wildcard $(top_srcdir)/po/*po) ; $(AM_V_GEN)$(INTLTOOL_MERGE) $(top_srcdir)/po $< $@ -d -u -c $(top_builddir)/po/.intltool-merge-cache

plugin_DATA = $(plugin_in_files:.eog-plugin.desktop.in=.eog-plugin)

EXTRA_DIST = $(plugin_in_files) $(UI_FILES)

CLEANFILES = $(plugin_DATA)
DISTCLEANFILES = $(plugin_DATA)

-include $(top_srcdir)/git.mk
