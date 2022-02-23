pythonconsole_sources = files(
  'config.py',
  'console.py',
  '__init__.py',
)

install_data(
  pythonconsole_sources,
  install_dir: pluginlibdir / 'pythonconsole',
)

install_data(
  files('config.ui'),
  install_dir: plugindatadir / 'pythonconsole'
)

# FIXME: Remove `args` parameter when gettext acquires plugin support.
#        http://lists.gnu.org/archive/html/bug-gettext/2017-06/msg00001.html
# NOTE:  We need to keep the .desktop suffix on the source file to be able
#        to extract translatable strings. .plugin is not recognized.
i18n.merge_file(
  input: 'pythonconsole.plugin.desktop.in',
  output: 'pythonconsole.plugin',
  po_dir: po_dir,
  type: 'desktop',
  args: ['--keyword=Name', '--keyword=Description'],
  install: true,
  install_dir: pluginlibdir,
)

configure_file(
  input: 'org.gnome.eog.plugins.pythonconsole.gschema.xml.in',
  output: '@BASENAME@',
  configuration: {'GETTEXT_PACKAGE': 'eog-plugins'},
  install: true,
  install_dir: gio_schemasdir,
)

pythonconsole_appdata = i18n.merge_file(
  input: 'eog-pythonconsole.appdata.xml.in',
  output: '@BASENAME@',
  po_dir: src_root / 'po',
  type: 'xml',
  install: true,
  install_dir: appstreamdir,
)

if appstream_util.found()
  test(
    'validate-eog-pythonconsole.appdata.xml',
    appstream_util,
    args: [
      'validate',
      '--nonet',
      pythonconsole_appdata.full_path(),
    ]
  )
endif

