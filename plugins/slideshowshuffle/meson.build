slideshowshuffle_sources = files(
  'slideshowshuffle.py',
)

install_data(
  slideshowshuffle_sources,
  install_dir: pluginlibdir,
)

# FIXME: Remove `args` parameter when gettext acquires plugin support.
#        http://lists.gnu.org/archive/html/bug-gettext/2017-06/msg00001.html
# NOTE:  We need to keep the .desktop suffix on the source file to be able
#        to extract translatable strings. .plugin is not recognized.
i18n.merge_file(
  input: 'slideshowshuffle.plugin.desktop.in',
  output: 'slideshowshuffle.plugin',
  po_dir: po_dir,
  type: 'desktop',
  args: ['--keyword=Name', '--keyword=Description'],
  install: true,
  install_dir: pluginlibdir,
)

slideshowshuffle_appdata = i18n.merge_file(
  input: 'eog-slideshowshuffle.appdata.xml.in',
  output: '@BASENAME@',
  po_dir: src_root / 'po',
  type: 'xml',
  install: true,
  install_dir: appstreamdir,
)

if appstream_util.found()
  test(
    'validate-eog-slideshowshuffle.appdata.xml',
    appstream_util,
    args: [
      'validate',
      '--nonet',
      slideshowshuffle_appdata.full_path(),
    ]
  )
endif

