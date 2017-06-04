# Champlain plugin
plugindir = $(libdir)/eog/plugins
appstreamdir = $(datadir)/appdata

AM_CPPFLAGS = \
	-I$(top_srcdir) 				\
	$(EOG_CFLAGS) 					\
	$(CHAMPLAIN_CFLAGS) 		\
	$(WARN_CFLAGS)					\
	-DEOG_LOCALEDIR=\""$(prefix)/$(DATADIRNAME)/locale"\"

plugin_LTLIBRARIES = libmap.la

libmap_la_SOURCES = \
	eog-map-plugin.h				\
	eog-map-plugin.c

libmap_la_LDFLAGS = \
	$(PLUGIN_LIBTOOL_FLAGS)				\
	$(EOG_LIBS) \
	$(CHAMPLAIN_LIBS)

# Plugin Info

plugin_in_files = map.plugin.desktop.in
plugin_DATA = $(plugin_in_files:.plugin.desktop.in=.plugin)
# msgfmt 0.19.8 has a bug that adding additional keywords for translation
# disables the default keywords, so we have to explicitly name them for now.
$(plugin_DATA): %.plugin: %.plugin.desktop.in $(wildcard $(top_srcdir)/po/*po)
	$(AM_V_GEN)$(MSGFMT) --desktop --keyword=Name --keyword=Description --template $< -d $(top_srcdir)/po -o $@

appstream_in_files = eog-map.metainfo.xml.in
appstream_DATA = $(appstream_in_files:.xml.in=.xml)
$(appstream_DATA): $(appstream_in_files)
	$(AM_V_GEN)$(MSGFMT) --xml --language=appdata --template $< -d $(top_srcdir)/po -o $@

EXTRA_DIST = $(plugin_in_files) $(appstream_in_files)

CLEANFILES = $(plugin_DATA) $(appstream_DATA)
DISTCLEANFILES = $(plugin_DATA) $(appstream_DATA)

-include $(top_srcdir)/git.mk
