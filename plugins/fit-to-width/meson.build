fittowidth_sources = files(
  'eog-fit-to-width-plugin.c',
)

fittowidth_deps = [
  eog_dep,
  libpeas_dep,
]

fittowidth_plugin = shared_module(
  'fit-to-width',
  sources: fittowidth_sources,
  include_directories: root_include_dir,
  dependencies: fittowidth_deps,
  install: true,
  install_dir: pluginlibdir,
)

# FIXME: Remove `args` parameter when gettext acquires plugin support.
#        http://lists.gnu.org/archive/html/bug-gettext/2017-06/msg00001.html
# NOTE:  We need to keep the .desktop suffix on the source file to be able
#        to extract translatable strings. .plugin is not recognized.
i18n.merge_file(
  input: 'fit-to-width.plugin.desktop.in',
  output: 'fit-to-width.plugin',
  po_dir: po_dir,
  type: 'desktop',
  args: ['--keyword=Name', '--keyword=Description'],
  install: true,
  install_dir: pluginlibdir,
)

fittowidth_appdata = i18n.merge_file(
  input: 'eog-fit-to-width.appdata.xml.in',
  output: '@BASENAME@',
  po_dir: src_root / 'po',
  type: 'xml',
  install: true,
  install_dir: appstreamdir,
)

if appstream_util.found()
  test(
    'validate-eog-fit-to-width.appdata.xml',
    appstream_util,
    args: [
      'validate',
      '--nonet',
      fittowidth_appdata.full_path(),
    ]
  )
endif

