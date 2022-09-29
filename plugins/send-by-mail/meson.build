sendbymail_sources = files(
  'eog-send-by-mail-plugin.c',
  'eog-send-by-mail-plugin.h',
)

sendbymail_deps = [
  eog_dep,
  libpeas_dep,
]

sendbymail_plugin = shared_module(
  'send-by-mail',
  sources: sendbymail_sources,
  include_directories: root_include_dir,
  dependencies: sendbymail_deps,
  install: true,
  install_dir: pluginlibdir,
)

# FIXME: Remove `args` parameter when gettext acquires plugin support.
#        http://lists.gnu.org/archive/html/bug-gettext/2017-06/msg00001.html
# NOTE:  We need to keep the .desktop suffix on the source file to be able
#        to extract translatable strings. .plugin is not recognized.
i18n.merge_file(
  input: 'send-by-mail.plugin.desktop.in',
  output: 'send-by-mail.plugin',
  po_dir: po_dir,
  type: 'desktop',
  args: ['--keyword=Name', '--keyword=Description'],
  install: true,
  install_dir: pluginlibdir,
)

sendbymail_appdata = i18n.merge_file(
  input: 'eog-send-by-mail.appdata.xml.in',
  output: '@BASENAME@',
  po_dir: src_root / 'po',
  type: 'xml',
  install: true,
  install_dir: appstreamdir,
)

if appstream_util.found()
  test(
    'validate-eog-send-by-mail.appdata.xml',
    appstream_util,
    args: [
      'validate',
      '--nonet',
      sendbymail_appdata.full_path(),
    ]
  )
endif

