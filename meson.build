project(
  'eog-plugins', 'c',
  version: '44.2',
  license: 'GPL2+',
  default_options: 'buildtype=debugoptimized',
  meson_version: '>= 0.58.0'
)

gnome = import('gnome')
i18n = import('i18n')
pkg = import('pkgconfig')
python = import('python')

# Paths
root_include_dir = include_directories('.')

src_root = meson.current_source_dir()
po_dir = src_root / 'po'

libdir = get_option('prefix') / get_option('libdir')
datadir = get_option('prefix') / get_option('datadir')

pkglibdir = libdir / 'eog'
pkgdatadir = datadir / 'eog'

pluginlibdir = pkglibdir / 'plugins'
plugindatadir = pkgdatadir / 'plugins'

appstreamdir = datadir / 'metainfo'
glibdir = datadir / 'glib-2.0'
localedir = datadir / 'locale'

gio_dep = dependency('gio-2.0', version: '>= 2.53.4')
gio_schemasdir = gio_dep.get_variable(
  'schemasdir',
  pkgconfig_define: ['datadir', datadir],
  default_value: glibdir / 'schemas',
)

# Dependencies in common for all plugins
libpeas_dep = dependency('libpeas-1.0', version: '>= 1.14.1')
libpeasgtk_dep = dependency('libpeas-gtk-1.0', version: '>= 1.14.1')

eog_dep = dependency('eog', version: '>= 41.0')

appstream_util = find_program('appstream-util', required: false)

source_root = meson.current_source_dir()
cc = meson.get_compiler('c')
common_flags = ['-DHAVE_CONFIG_H']
compiler_flags = []
if get_option('buildtype').contains('debug')
  compiler_flags += cc.get_supported_arguments([
    '-Werror=format=2',
    '-Werror=implicit-function-declaration',
    '-Werror=init-self',
    '-Werror=missing-include-dirs',
    '-Werror=missing-prototypes',
    '-Werror=pointer-arith',
    '-Werror=return-type',
    '-Wnested-externs',
    '-Wstrict-prototypes',
  ])
endif
add_project_arguments(common_flags + compiler_flags, language: 'c')

# config.h
config_h = configuration_data()
config_h.set_quoted('GETTEXT_PACKAGE', meson.project_name())

# Options
enabled_plugins = []
disabled_plugins = []
extra_languages = []
all_plugins = {
  'exif-display': {'language': 'c'},
  'export-to-folder': {'language': 'python'},
  'fit-to-width': {'language': 'c'},
  'fullscreenbg': {'language': 'python'},
  'light-theme': {'language': 'c'},
  'map': {'language': 'c'},
  'maximize-windows': {'language': 'c'},
  'postasa': {'language': 'c'},
  'postr': {'language': 'c'},
  'pythonconsole': {'language': 'python'},
  'send-by-mail': {'language': 'c'},
  'slideshowshuffle': {'language': 'python'},
}

foreach plugin_name, plugin_metadata : all_plugins
  if get_option('plugin_@0@'.format(plugin_name))
    enabled_plugins += plugin_name

    plugin_language = plugin_metadata.get('language')
    if plugin_language != 'c'
      extra_languages += plugin_language
    endif
  else
    disabled_plugins += plugin_name
  endif
endforeach

if 'python' in extra_languages
  python3 = python.find_installation('python3')
endif

subdir('po')
subdir('plugins')

configure_file(
  output: 'config.h',
  configuration: config_h
)

gnome.post_install(
  glib_compile_schemas: true,
)

# Summary message

summary({
  'Prefix': get_option('prefix'),
  'Compiler': cc.get_id(),
  'Extra compiler warnings': compiler_flags,
}, section: 'Build')

summary({
  'Enabled plugins': enabled_plugins,
  'Disabled plugins': disabled_plugins,
}, section: 'Plugins')
