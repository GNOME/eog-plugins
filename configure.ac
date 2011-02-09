 Process this file with autoconf to produce a configure script.

EOG_MAJOR=3.0.0
EOG_PLUGINS_MAJOR=3.0.0

AC_INIT([eog-plugins], [2.91.7], [http://bugzilla.gnome.org/enter_bug.cgi?product=eog&component=plugins])

AC_CONFIG_SRCDIR([plugins])
AC_PREFIX_PROGRAM([eog])

AC_SUBST(EOG_PLUGINS_MAJOR)

AC_PREREQ(2.59)

AM_INIT_AUTOMAKE([1.9 foreign dist-bzip2 no-dist-gzip])
# Support silencing the build output if supported (automake-1.11+)
m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])

AM_MAINTAINER_MODE
AC_CONFIG_HEADERS([config.h])
AC_DISABLE_STATIC

AC_PROG_LIBTOOL

AC_ISC_POSIX
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET

# ***********
# Translation 
# ***********

AM_GLIB_GNU_GETTEXT
IT_PROG_INTLTOOL([0.40.0])

GETTEXT_PACKAGE=eog-plugins
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE", [Gettext package])

# **********
# GNOME Libs 
# **********

PKG_CHECK_MODULES([EOG], [
	glib-2.0 >= 2.26.0
	gtk+-3.0 >= 2.99.3
	libpeas-1.0 >= 0.7.0
	eog >= 2.91.6
])
AC_SUBST(EOG_LIBS)
AC_SUBST(EOG_CFLAGS)

# *******
# Plugins
# *******

#FIXME
EOG_HAS_PYTHON=1
#/FIXME

AC_MSG_CHECKING([which plugins to build])

ALL_PLUGINS="postr map fit-to-width exif-display send-by-mail postasa"
USEFUL_PLUGINS="postr map fit-to-width exif-display send-by-mail postasa"
DEFAULT_PLUGINS="postr map fit-to-width exif-display send-by-mail postasa"

PYTHON_ALL_PLUGINS="slideshowshuffle pythonconsole"
PYTHON_USEFUL_PLUGINS="slideshowshuffle pythonconsole"
PYTHON_DEFAULT_PLUGINS="slideshowshuffle pythonconsole"

DIST_PLUGINS="$ALL_PLUGINS $PYTHON_ALL_PLUGINS"

if test $EOG_HAS_PYTHON
then
	ALL_PLUGINS="$ALL_PLUGINS $PYTHON_ALL_PLUGINS"
	USEFUL_PLUGINS="$USEFUL_PLUGINS $PYTHON_USEFUL_PLUGINS"
	DEFAULT_PLUGINS="$DEFAULT_PLUGINS $PYTHON_DEFAULT_PLUGINS"
else
	AC_MSG_WARN([eog compiled without python support, plugins $PYTHON_ALL_PLUGINS will not be available])
fi
AC_ARG_WITH([plugins],
	    [AS_HELP_STRING([--with-plugins=LIST],
			    [Comma-separated list specifying the plugins to build.
			    Available: map, exif-display, fit-to-width,
			    postr, postasa, pythonconsole, slideshowshuffle,
			    as well as the aliases default,
			    all, and really-all])],
	    [plugins=$with_plugins],
	    [plugins="default"])

if test "x$with_plugins" = xyes
then
	plugins="default"
fi

need_python=no
explicit_plugins=
PLUGINS=
IFS="${IFS=	}"; eogpl_save_ifs="$IFS"; IFS=","
for pl in $plugins
do
	if echo "$ALL_PLUGINS" | egrep "(^| )$pl(\$| )" > /dev/null
	then
		PLUGINS="$PLUGINS $pl"
		explicit_plugins="$explicit_plugins $pl"
	elif test "x$pl" = "xdefault"
	then
		PLUGINS="$PLUGINS $DEFAULT_PLUGINS"
	elif test "x$pl" = "xall"
	then
		PLUGINS="$PLUGINS $USEFUL_PLUGINS"
	elif test "x$pl" = "xreally-all"
	then
		PLUGINS="$PLUGINS $ALL_PLUGINS"
	else
		IFS=$eogpl_save_ifs
		AC_MSG_ERROR([the specified plugin $pl does not exist])
	fi
	
	if echo "$PYTHON_ALL_PLUGINS" | egrep "(^| )$pl(\$| )" > /dev/null
	then
		need_python=yes
	fi
done
IFS="$eogpl_save_ifs"

AC_MSG_RESULT([$PLUGINS])

# *****
# GSettings 
# *****

GLIB_GSETTINGS

# **************
# Python Support 
# **************

AC_MSG_CHECKING([whether Python support is requested])

AC_ARG_ENABLE([python],
	AS_HELP_STRING([--enable-python],[Enable python support]),
	[enable_python=$enableval have_python=$enableval],
	[enable_python=autodetect have_python=yes])

AC_MSG_RESULT([$enable_python])

if test "x$have_python" != "xno"; then
	AM_PATH_PYTHON([2.3],[],[no])
	if test "x$PYTHON" = "x:"; then
		have_python=no
	fi
fi

if test "x$have_python" != "xno"; then
	AC_MSG_NOTICE([Disabling Python plugins as they do not work yet!])
	have_python=no
	enable_python=no
fi

if test "x$have_python" != "xno"; then
	PY_PREFIX=`$PYTHON -c 'import sys ; print sys.prefix'`
	PY_EXEC_PREFIX=`$PYTHON -c 'import sys ; print sys.exec_prefix'`
	PYTHON_LIBS="-lpython$PYTHON_VERSION"
	PYTHON_LIB_LOC="-L$PY_EXEC_PREFIX/lib/python$PYTHON_VERSION/config"
	PYTHON_CFLAGS="-I$PY_PREFIX/include/python$PYTHON_VERSION"
	PYTHON_MAKEFILE="$PY_EXEC_PREFIX/lib/python$PYTHON_VERSION/config/Makefile"
	PYTHON_LOCALMODLIBS=`sed -n -e 's/^LOCALMODLIBS=\(.*\)/\1/p' $PYTHON_MAKEFILE`
	PYTHON_BASEMODLIBS=`sed -n -e 's/^BASEMODLIBS=\(.*\)/\1/p' $PYTHON_MAKEFILE`
	PYTHON_OTHER_LIBS=`sed -n -e 's/^LIBS=\(.*\)/\1/p' $PYTHON_MAKEFILE`
	PYTHON_EXTRA_LIBS="$PYTHON_LOCALMODLIBS $PYTHON_BASEMODLIBS $PYTHON_OTHER_LIBS"
	AC_SUBST([PYTHON_LIBS])
	AC_SUBST([PYTHON_LIB_LOC])
	AC_SUBST([PYTHON_CFLAGS])
	AC_SUBST([PYTHON_EXTRA_LIBS])
fi

if test "x$have_python" != "xno"; then
	PYGOBJECT_REQUIRED=2.11.5
	PYGTK_REQUIRED=2.9.7
	GNOME_PYTHON_REQUIRED=2.18.2

	PKG_CHECK_MODULES([PYGTK], [
		pygobject-2.0 >= $PYGOBJECT_REQUIRED 
		gnome-python-2.0 >= $GNOME_PYTHON_REQUIRED
		pygtk-2.0 >= $PYGTK_REQUIRED],
		[],[have_python=no])

	AC_SUBST([PYGTK_CFLAGS])
	AC_SUBST([PYGTK_LIBS])
fi

if test "x$have_python" != "xno"; then
	AC_MSG_CHECKING([for pygtk defs])
	PYGTK_DEFSDIR=`$PKG_CONFIG --variable=defsdir pygtk-2.0`
	AC_MSG_RESULT([$PYGTK_DEFSDIR])

	AC_MSG_CHECKING([for pygtk codegen])
	PYGTK_CODEGEN="$PYTHON `$PKG_CONFIG --variable=codegendir pygtk-2.0`/codegen.py"
	AC_MSG_RESULT([$PYGTK_CODEGEN])

	AC_MSG_CHECKING([for pygtk h2def])
	PYGTK_H2DEF="$PYTHON `$PKG_CONFIG --variable=codegendir pygtk-2.0`/h2def.py"
	AC_MSG_RESULT([$PYGTK_H2DEF])

	AC_SUBST([PYGTK_DEFSDIR])
	AC_SUBST([PYGTK_CODEGEN])
	AC_SUBST([PYGTK_H2DEF])

	dnl Check for -fno-strict-aliasing
	FLAGS="-fno-strict-aliasing"
	save_CFLAGS="$CFLAGS"
	CFLAGS="$CFLAGS $FLAGS"
	AC_MSG_CHECKING([whether [$]CC understands $FLAGS])
	AC_TRY_COMPILE([], [], [compiler_has_option=yes], [compiler_has_option=no])
	CFLAGS="$save_CFLAGS"
	AC_MSG_RESULT($compiler_has_option)
	if test $compiler_has_option = yes; then
		NO_STRICT_ALIASING_CFLAGS="$FLAGS"
	fi
	AC_SUBST([NO_STRICT_ALIASING_CFLAGS])
fi

if test "x$have_python" != "xyes"; then
	if test "x$enable_python" = "xyes"; then
		AC_MSG_ERROR([Python not found])
	elif test "x$enable_python" = "xautodetect"; then
		enable_python=no
		AC_MSG_WARN([Python not found, disabling python support])
	fi
elif test "x$enable_python" != "xno"; then
	enable_python=yes
	AC_DEFINE([ENABLE_PYTHON],[1],[Define to compile with python support])
fi

AM_CONDITIONAL([ENABLE_PYTHON],[test "x$enable_python" = "xyes"])

disabled_plugins=

# *******************
# Plugin Dependencies
# *******************

check_plugin_defined() {
	if echo "$2" | egrep "\b($1)\b" > /dev/null;
	then
		return 1
	else
		return 0
	fi
}

plugin_defined() {
	check_plugin_defined "$1" "$PLUGINS"
	return $?
}

plugin_defined_explicit() {
	check_plugin_defined "$1" "$explicit_plugins"
	return $?
}

undef_plugin() {
	if test -z "$disabled_plugins"
	then
		disabled_plugins="$1 ($2)"
	else
		disabled_plugins="$disabled_plugins, $1 ($2)"
	fi
	
	PLUGINS=$(echo "$PLUGINS" | sed -e "s/[[^a-zA-Z0-9_]]$1[[ \t\r\n\f]]\{1,\}/ /" -e "s/[[^a-zA-Z0-9_]]$1$//" -e "s/^$1[[ \t\r\n\f]]\{1,\}//")
}

# Disable python plugins if there is no python support
if test "x$enable_python" = "xno"
then
	for pl in $PYTHON_ALL_PLUGINS
	do
		undef_plugin "$pl" "no python support"
	done
fi

# Disable plugins that are not yet ported to eog-3.0
UNPORTED_PLUGINS="map postasa"
for pl in $UNPORTED_PLUGINS
do
	undef_plugin "$pl" "not yet ported to 3.0"
done

# *****
# Postr 
# *****
plugin_defined postr
if test "$?" = 1
then
	AC_PATH_PROG(POSTR, postr)
	if test -z "$POSTR"
	then
		undef_plugin postr "postr was not found"
	fi
fi

# *******
# Postasa
# *******
plugin_defined postasa
if test "$?" = 1
then
    PKG_CHECK_MODULES(POSTASA,
        [ libgdata >= 0.6.0 ],
        PKG_CHECK_EXISTS( libgdata >= 0.8 ,
	    AC_DEFINE([HAVE_LIBGDATA_80], [1],
	        [Define to be compatible with the API of libgdata-0.8])
	),
	[have_postasa=no])
    AC_SUBST(POSTASA_LIBS)
    AC_SUBST(POSTASA_CFLAGS)
    if test "x$have_postasa" = "xno"
    then
        undef_plugin postasa "libgdata was not found"
    fi
fi

# ***
# Map
# ***
plugin_defined map
if test "$?" = 1
then
	
	PKG_CHECK_MODULES(CHAMPLAIN,
		[ champlain-0.8 >= 0.7.1,
		  champlain-gtk-0.8 >= 0.7.1,
		  clutter-gtk-0.10 >= 0.10,
		  libexif >= 0.6.16 ],
		[],[have_libchamplain=no])
	AC_SUBST(CHAMPLAIN_LIBS)
	AC_SUBST(CHAMPLAIN_CFLAGS)
	if test "x$have_libchamplain" = "xno"
	then
		undef_plugin map "libchamplain or libexif was not found"
	fi
fi

# ***********
# ExifDisplay
# ***********
plugin_defined exif-display
if test "$?" = 1
then

    PKG_CHECK_MODULES(EXIFDISPLAY,
        [ libexif >= 0.6.16 ],
        [],[have_exifdisplay=no])
    AC_SUBST(EXIFDISPLAY_LIBS)
    AC_SUBST(EXIFDISPLAY_CFLAGS)
    if test "x$have_exifdisplay" = "xno"
    then
        undef_plugin exif-display "libexif was not found"
    fi
fi


if test -z "$disabled_plugins"
then
	disabled_plugins="none"
fi

PLUGINS=$(echo $PLUGINS | tr ' ' '\n' | sort | uniq | tr '\n' ' ')
AC_SUBST([DIST_PLUGINS])
AC_SUBST([BUILD_PLUGINS],[$PLUGINS])

# ****
# Misc 
# ****
AC_PATH_PROG(GLIB_GENMARSHAL, glib-genmarshal)

PLUGIN_LIBTOOL_FLAGS="-module -avoid-version"
AC_SUBST(PLUGIN_LIBTOOL_FLAGS)

AC_CONFIG_FILES([
Makefile
plugins/Makefile
plugins/map/Makefile
plugins/fit-to-width/Makefile
plugins/exif-display/Makefile
plugins/exif-display/org.gnome.eog.plugins.exif-display.gschema.xml.in
plugins/send-by-mail/Makefile
plugins/slideshowshuffle/Makefile
plugins/postr/Makefile
plugins/postasa/Makefile
plugins/pythonconsole/Makefile
po/Makefile.in])

AC_OUTPUT

echo "
Configure summary:

	Source code location:..: ${srcdir}
	Compiler...............: ${CC}
	Prefix ................: ${prefix}
	Python Support ........: $enable_python
	Plugins ...............: $PLUGINS
	Disabled plugins ......: $disabled_plugins

Note: you have to install these plugins into the same prefix than your eog
installation (probably /usr if you're using your distro packages, /usr/local
if you have compiled it on your own).
"
