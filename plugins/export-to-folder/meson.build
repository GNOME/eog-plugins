exporttofolder_sources = files(
  'export-to-folder.py',
)

install_data(
  exporttofolder_sources,
  install_dir: pluginlibdir,
)

install_data(
  files('preferences_dialog.ui'),
  install_dir: plugindatadir / 'export-to-folder'
)

# FIXME: Remove `args` parameter when gettext acquires plugin support.
#        http://lists.gnu.org/archive/html/bug-gettext/2017-06/msg00001.html
# NOTE:  We need to keep the .desktop suffix on the source file to be able
#        to extract translatable strings. .plugin is not recognized.
i18n.merge_file(
  input: 'export-to-folder.plugin.desktop.in',
  output: 'export-to-folder.plugin',
  po_dir: po_dir,
  type: 'desktop',
  args: ['--keyword=Name', '--keyword=Description'],
  install: true,
  install_dir: pluginlibdir,
)

configure_file(
  input: 'org.gnome.eog.plugins.export-to-folder.gschema.xml.in',
  output: '@BASENAME@',
  configuration: {'GETTEXT_PACKAGE': 'eog-plugins'},
  install: true,
  install_dir: gio_schemasdir,
)

exporttofolder_appdata = i18n.merge_file(
  input: 'eog-export-to-folder.appdata.xml.in',
  output: '@BASENAME@',
  po_dir: src_root / 'po',
  type: 'xml',
  install: true,
  install_dir: appstreamdir,
)

if appstream_util.found()
  test(
    'validate-eog-export-to-folder.appdata.xml',
    appstream_util,
    args: [
      'validate',
      '--nonet',
      exporttofolder_appdata.full_path(),
    ]
  )
endif

