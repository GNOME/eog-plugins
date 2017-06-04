# Postasa plugin
plugindir = $(EOG_PLUGINS_LIBS_DIR)
appstreamdir = $(datadir)/appdata

AM_CPPFLAGS = \
	-I$(top_srcdir) 				\
	$(EOG_CFLAGS) 					\
	$(WARN_CFLAGS)					\
	$(POSTASA_CFLAGS) 				\
	-DEOG_LOCALEDIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DEOG_POSTASA_PLUGIN_DIR=\""$(plugindir)"\"		\
	-DEOG_POSTASA_DATA_DIR=\""$(EOG_PLUGINS_DATA_DIR)/postasa"\"


plugin_LTLIBRARIES = libpostasa.la

BUILT_SOURCES = \
	eog-postasa-plugin-resources.c

libpostasa_la_SOURCES = \
	${BUILT_SOURCES}				\
	eog-postasa-plugin.h				\
	eog-postasa-plugin.c

libpostasa_la_LDFLAGS = \
	$(PLUGIN_LIBTOOL_FLAGS)				\
	$(EOG_LIBS)

libpostasa_la_LIBADD = $(EOG_LIBS)			\
	$(POSTASA_LIBS)

eog-postasa-plugin-resources.c: eog-postasa-plugin.gresource.xml $(shell $(GLIB_COMPILE_RESOURCES) --sourcedir=$(srcdir) --generate-dependencies $(srcdir)/eog-postasa-plugin.gresource.xml)
	$(AM_V_GEN) $(GLIB_COMPILE_RESOURCES) --internal --target=$@ --sourcedir=$(srcdir) --generate-source $(srcdir)/eog-postasa-plugin.gresource.xml

UI_FILES = postasa-config.xml postasa-uploads.xml

# Plugin Info

plugin_in_files = postasa.plugin.desktop.in
plugin_DATA = $(plugin_in_files:.plugin.desktop.in=.plugin)
# msgfmt 0.19.8 has a bug that adding additional keywords for translation
# disables the default keywords, so we have to explicitly name them for now.
$(plugin_DATA): %.plugin: %.plugin.desktop.in $(wildcard $(top_srcdir)/po/*po)
	$(AM_V_GEN)$(MSGFMT) --desktop --keyword=Name --keyword=Description --template $< -d $(top_srcdir)/po -o $@

appstream_in_files = eog-postasa.appdata.xml.in
appstream_DATA = $(appstream_in_files:.appdata.xml.in=.metainfo.xml)
$(appstream_DATA): $(appstream_in_files)
	$(AM_V_GEN)$(MSGFMT) --xml --language=appdata --template $< -d $(top_srcdir)/po -o $@


EXTRA_DIST = \
	$(plugin_in_files)	\
	$(appstream_in_files)	\
	$(UI_FILES)		\
	eog-postasa-plugin.gresource.xml

CLEANFILES = $(plugin_DATA) $(appstream_DATA) $(BUILT_SOURCES)
DISTCLEANFILES = $(plugin_DATA) $(appstream_DATA) $(BUILT_SOURCES)

-include $(top_srcdir)/git.mk
