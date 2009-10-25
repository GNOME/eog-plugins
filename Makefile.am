SUBDIRS = plugins po

EXTRA_DIST =			\
	autogen.sh		\
	ChangeLog

DISTCLEANFILES = 		\
	intltool-extract 	\
	intltool-merge 		\
	intltool-update

MAINTAINERCLEANFILES =		\
	$(srcdir)/aclocal.m4	\
	$(srcdir)/config.guess	\
	$(srcdir)/config.h.in	\
	$(srcdir)/config.sub	\
	$(srcdir)/depcomp	\
	$(srcdir)/install-sh	\
	$(srcdir)/ltmain.sh	\
	$(srcdir)/missing	\
	$(srcdir)/mkinstalldirs

DISTCHECK_CONFIGURE_FLAGS = --with-plugins=really-all

-include $(top_srcdir)/git.mk
