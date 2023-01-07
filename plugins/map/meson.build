map_sources = files(
  'eog-map-plugin.c',
)

map_deps = [
  eog_dep,
  libpeas_dep,
  libpeasgtk_dep,
  dependency('champlain-0.12', version: '>= 0.9.0'),
  dependency('champlain-gtk-0.12', version: '>= 0.9.0'),
  dependency('clutter-1.0', version: '>= 1.9.4'),
  dependency('clutter-gtk-1.0', version: '>= 1.1.2'),
  dependency('libexif', version: '>= 0.6.16'),
]

map_plugin = shared_module(
  'map',
  sources: map_sources,
  include_directories: root_include_dir,
  dependencies: map_deps,
  install: true,
  install_dir: pluginlibdir,
)

# FIXME: Remove `args` parameter when gettext acquires plugin support.
#        http://lists.gnu.org/archive/html/bug-gettext/2017-06/msg00001.html
# NOTE:  We need to keep the .desktop suffix on the source file to be able
#        to extract translatable strings. .plugin is not recognized.
i18n.merge_file(
  input: 'map.plugin.desktop.in',
  output: 'map.plugin',
  po_dir: po_dir,
  type: 'desktop',
  args: ['--keyword=Name', '--keyword=Description'],
  install: true,
  install_dir: pluginlibdir,
)

map_appdata = i18n.merge_file(
  input: 'eog-map.appdata.xml.in',
  output: '@BASENAME@',
  po_dir: src_root / 'po',
  type: 'xml',
  install: true,
  install_dir: appstreamdir,
)

if appstream_util.found()
  test(
    'validate-eog-map.appdata.xml.in',
    appstream_util,
    args: [
      'validate',
      '--nonet',
      map_appdata.full_path(),
    ]
  )
endif

