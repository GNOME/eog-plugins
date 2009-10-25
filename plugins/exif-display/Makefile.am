# ExifDisplay plugin
plugindir = $(libdir)/eog/plugins

INCLUDES = \
	-I$(top_srcdir)/src 				\
	$(EOG_CFLAGS) 					\
	$(WARN_CFLAGS)					\
	$(EXIFDISPLAY_CFLAGS)				\
	-DEOG_LOCALEDIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DEOG_PLUGINDIR=\""$(plugindir)"\"

plugin_LTLIBRARIES = libexif-display.la

libexif_display_la_SOURCES = \
	eog-exif-display-plugin.h			\
	eog-exif-display-plugin.c

libexif_display_la_LDFLAGS = \
	-avoid-version -module

libexif_display_la_LIBADD = $(EOG_LIBS)			\
	$(EXIFDISPLAY_LIBS)

# Plugin Info

plugin_in_files = exif-display.eog-plugin.desktop.in

UI_FILES = exif-display.ui exif-display-config.ui
ui_DATA = $(UI_FILES)
uidir = $(plugindir)/exif-display

%.eog-plugin: %.eog-plugin.desktop.in $(INTLTOOL_MERGE) $(wildcard $(top_srcdir)/po/*po) ; $(AM_V_GEN)$(INTLTOOL_MERGE) $(top_srcdir)/po $< $@ -d -u -c $(top_builddir)/po/.intltool-merge-cache

plugin_DATA = $(plugin_in_files:.eog-plugin.desktop.in=.eog-plugin)

EXTRA_DIST = $(plugin_in_files) $(UI_FILES)

CLEANFILES = $(plugin_DATA)
DISTCLEANFILES = $(plugin_DATA)

-include $(top_srcdir)/git.mk
