# SendByMail plugin
plugindir = $(libdir)/eog/plugins

AM_CPPFLAGS = \
	-I$(top_srcdir)/src 				\
	$(EOG_CFLAGS) 					\
	$(WARN_CFLAGS)					\
	-DEOG_LOCALEDIR=\""$(prefix)/$(DATADIRNAME)/locale"\"

plugin_LTLIBRARIES = libsend-by-mail.la

libsend_by_mail_la_SOURCES = \
	eog-send-by-mail-plugin.h			\
	eog-send-by-mail-plugin.c

libsend_by_mail_la_LDFLAGS = \
	-avoid-version -module

libsend_by_mail_la_LIBADD = $(EOG_LIBS)

# Plugin Info

plugin_in_files = send-by-mail.plugin.desktop.in

%.plugin: %.plugin.desktop.in $(INTLTOOL_MERGE) $(wildcard $(top_srcdir)/po/*po) ; $(AM_V_GEN)$(INTLTOOL_MERGE) $(top_srcdir)/po $< $@ -d -u -c $(top_builddir)/po/.intltool-merge-cache

plugin_DATA = $(plugin_in_files:.plugin.desktop.in=.plugin)

EXTRA_DIST = $(plugin_in_files)

CLEANFILES = $(plugin_DATA)
DISTCLEANFILES = $(plugin_DATA)

-include $(top_srcdir)/git.mk
