maximizewindows_sources = files(
  'maximize-windows.py',
)

install_data(
  maximizewindows_sources,
  install_dir: pluginlibdir,
)

# FIXME: Remove `args` parameter when gettext acquires plugin support.
#        http://lists.gnu.org/archive/html/bug-gettext/2017-06/msg00001.html
# NOTE:  We need to keep the .desktop suffix on the source file to be able
#        to extract translatable strings. .plugin is not recognized.
i18n.merge_file(
  input: 'maximize-windows.plugin.desktop.in',
  output: 'maximize-windows.plugin',
  po_dir: po_dir,
  type: 'desktop',
  args: ['--keyword=Name', '--keyword=Description'],
  install: true,
  install_dir: pluginlibdir,
)

maximizewindows_appdata = i18n.merge_file(
  input: 'eog-maximize-windows.appdata.xml.in',
  output: '@BASENAME@',
  po_dir: src_root / 'po',
  type: 'xml',
  install: true,
  install_dir: appstreamdir,
)

if appstream_util.found()
  test(
    'validate-eog-maximize-windows.appdata.xml',
    appstream_util,
    args: [
      'validate',
      '--nonet',
      maximizewindows_appdata.full_path(),
    ]
  )
endif

