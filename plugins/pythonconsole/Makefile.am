# Python Console plugin
plugindir = $(EOG_PLUGINS_LIBS_DIR)
appstreamdir = $(datadir)/appdata
pythonconsoledir = $(plugindir)/pythonconsole
uidir = $(EOG_PLUGINS_DATA_DIR)/pythonconsole
plugin_in_files = pythonconsole.plugin.desktop.in
appstream_in_files = eog-pythonconsole.metainfo.xml.in

pythonconsole_PYTHON =	\
	__init__.py	\
	config.py	\
	console.py

UI_FILES = config.ui
ui_DATA = $(UI_FILES)

plugin_DATA = $(plugin_in_files:.plugin.desktop.in=.plugin)
# msgfmt 0.19.8 has a bug that adding additional keywords for translation
# disables the default keywords, so we have to explicitly name them for now.
$(plugin_DATA): %.plugin: %.plugin.desktop.in $(wildcard $(top_srcdir)/po/*po)
	$(AM_V_GEN)$(MSGFMT) --desktop --keyword=Name --keyword=Description --template $< -d $(top_srcdir)/po -o $@

appstream_DATA = $(appstream_in_files:.xml.in=.xml)
$(appstream_DATA): $(appstream_in_files)
	$(AM_V_GEN)$(MSGFMT) --xml --language=appdata --template $< -d $(top_srcdir)/po -o $@


gsettings_SCHEMAS = org.gnome.eog.plugins.pythonconsole.gschema.xml
@GSETTINGS_RULES@

EXTRA_DIST = \
	$(plugin_in_files)	\
	$(appstream_in_files)	\
	$(UI_FILES)		\
	org.gnome.eog.plugins.pythonconsole.gschema.xml.in


CLEANFILES = $(plugin_DATA) $(appstream_DATA) $(gsettings_SCHEMAS)
DISTCLEANFILES = $(plugin_DATA) $(appstream_DATA) $(gsettings_SCHEMAS)
MAINTAINERCLEANFILES = $(gsettings_SCHEMAS:.xml=.valid)

-include $(top_srcdir)/git.mk
