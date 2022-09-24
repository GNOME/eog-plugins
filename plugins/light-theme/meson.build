lighttheme_sources = files(
  'eog-light-theme-plugin.c',
)

lighttheme_deps = [
  eog_dep,
  libpeas_dep,
]

lighttheme_plugin = shared_module(
  'light-theme',
  sources: lighttheme_sources,
  include_directories: root_include_dir,
  dependencies: lighttheme_deps,
  install: true,
  install_dir: pluginlibdir,
)

# FIXME: Remove `args` parameter when gettext acquires plugin support.
#        http://lists.gnu.org/archive/html/bug-gettext/2017-06/msg00001.html
# NOTE:  We need to keep the .desktop suffix on the source file to be able
#        to extract translatable strings. .plugin is not recognized.
i18n.merge_file(
  input: 'light-theme.plugin.desktop.in',
  output: 'light-theme.plugin',
  po_dir: po_dir,
  type: 'desktop',
  args: ['--keyword=Name', '--keyword=Description'],
  install: true,
  install_dir: pluginlibdir,
)

lighttheme_appdata = i18n.merge_file(
  input: 'eog-light-theme.appdata.xml.in',
  output: '@BASENAME@',
  po_dir: src_root / 'po',
  type: 'xml',
  install: true,
  install_dir: appstreamdir,
)

if appstream_util.found()
  test(
    'validate-eog-light-theme.appdata.xml',
    appstream_util,
    args: [
      'validate',
      '--nonet',
      lighttheme_appdata.full_path(),
    ]
  )
endif

