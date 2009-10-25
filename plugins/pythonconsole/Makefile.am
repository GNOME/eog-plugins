# Python Console plugin
plugindir = $(libdir)/eog/plugins
plugin_in_files = pythonconsole.eog-plugin.desktop.in

plugin_PYTHON = \
	pythonconsole.py \
	console.py

%.eog-plugin: %.eog-plugin.desktop.in $(INTLTOOL_MERGE) $(wildcard $(top_srcdir)/po/*po) ; $(AM_V_GEN)$(INTLTOOL_MERGE) $(top_srcdir)/po $< $@ -d -u -c $(top_builddir)/po/.intltool-merge-cache

plugin_DATA = $(plugin_in_files:.eog-plugin.desktop.in=.eog-plugin)

EXTRA_DIST = $(plugin_in_files)

CLEANFILES = $(plugin_DATA)
DISTCLEANFILES = $(plugin_DATA)

-include $(top_srcdir)/git.mk
