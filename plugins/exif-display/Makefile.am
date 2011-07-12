# ExifDisplay plugin
plugindir = $(EOG_PLUGINS_LIBS_DIR)
uidir = $(EOG_PLUGINS_DATA_DIR)/exif-display

INCLUDES = \
	-I$(top_srcdir)/src 				\
	$(EOG_CFLAGS) 					\
	$(WARN_CFLAGS)					\
	$(EXIFDISPLAY_CFLAGS)				\
	-DEOG_LOCALEDIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DEOG_EXIF_DISPLAY_DATA_DIR=\""$(uidir)"\"

plugin_LTLIBRARIES = libexif-display.la

libexif_display_la_SOURCES = \
	eog-exif-display-plugin.h			\
	eog-exif-display-plugin.c			\
	eog-exif-display-plugin-settings.h		\
	eog-exif-display-plugin-setup.c			\
	eog-exif-display-plugin-setup.h

libexif_display_la_LDFLAGS = \
	-avoid-version -module

libexif_display_la_LIBADD = $(EOG_LIBS)			\
	$(EXIFDISPLAY_LIBS)

# Plugin Info

plugin_in_files = exif-display.plugin.desktop.in

UI_FILES = exif-display.ui exif-display-config.ui
ui_DATA = $(UI_FILES)

%.plugin: %.plugin.desktop.in $(INTLTOOL_MERGE) $(wildcard $(top_srcdir)/po/*po) ; $(AM_V_GEN)$(INTLTOOL_MERGE) $(top_srcdir)/po $< $@ -d -u -c $(top_builddir)/po/.intltool-merge-cache

plugin_DATA = $(plugin_in_files:.plugin.desktop.in=.plugin)

gsettings_SCHEMAS = org.gnome.eog.plugins.exif-display.gschema.xml
@INTLTOOL_XML_NOMERGE_RULE@
@GSETTINGS_RULES@

EXTRA_DIST = \
	$(plugin_in_files)	\
	$(UI_FILES)		\
	org.gnome.eog.plugins.exif-display.gschema.xml.in.in

CLEANFILES = $(plugin_DATA) $(gsettings_SCHEMAS)
DISTCLEANFILES = $(plugin_DATA) $(gsettings_SCHEMAS)
MAINTAINERCLEANFILES = $(gsettings_SCHEMAS:.xml=.valid)

-include $(top_srcdir)/git.mk
