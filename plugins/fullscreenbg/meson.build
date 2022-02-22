fullscreenbg_sources = files(
  'fullscreenbg.py',
)

install_data(
  fullscreenbg_sources,
  install_dir: pluginlibdir,
)

install_data(
  files('preferences_dialog.ui'),
  install_dir: plugindatadir / 'fullscreenbg'
)

# FIXME: Remove `args` parameter when gettext acquires plugin support.
#        http://lists.gnu.org/archive/html/bug-gettext/2017-06/msg00001.html
# NOTE:  We need to keep the .desktop suffix on the source file to be able
#        to extract translatable strings. .plugin is not recognized.
i18n.merge_file(
  input: 'fullscreenbg.plugin.desktop.in',
  output: 'fullscreenbg.plugin',
  po_dir: po_dir,
  type: 'desktop',
  args: ['--keyword=Name', '--keyword=Description'],
  install: true,
  install_dir: pluginlibdir,
)

configure_file(
  input: 'org.gnome.eog.plugins.fullscreenbg.gschema.xml.in',
  output: '@BASENAME@',
  configuration: {'GETTEXT_PACKAGE': 'eog-plugins'},
  install: true,
  install_dir: gio_schemasdir,
)

fullscreenbg_appdata = i18n.merge_file(
  input: 'eog-fullscreenbg.appdata.xml.in',
  output: '@BASENAME@',
  po_dir: src_root / 'po',
  type: 'xml',
  install: true,
  install_dir: appstreamdir,
)

if appstream_util.found()
  test(
    'validate-eog-fullscreenbg.appdata.xml',
    appstream_util,
    args: [
      'validate',
      '--nonet',
      fullscreenbg_appdata.full_path(),
    ]
  )
endif

