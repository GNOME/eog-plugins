exifdisplay_sources = files(
  'eog-exif-display-plugin.c',
  'eog-exif-display-plugin-setup.c',
)

exifdisplay_res = gnome.compile_resources(
  'eog-exif-display-resources',
  'eog-exif-display-plugin.gresource.xml',
)

exifdisplay_sources += [ exifdisplay_res.get(0) ]

exifdisplay_deps = [
  eog_dep,
  libpeas_dep,
  libpeasgtk_dep,
  dependency('libexif', version: '>= 0.6.16'),
]

exifdisplay_sha = shared_module(
  'exif-display',
  sources: exifdisplay_sources,
  include_directories: root_include_dir,
  dependencies: exifdisplay_deps,
  install: true,
  install_dir: pluginlibdir,
)

# FIXME: Remove `args` parameter when gettext acquires plugin support.
#        http://lists.gnu.org/archive/html/bug-gettext/2017-06/msg00001.html
# NOTE:  We need to keep the .desktop suffix on the source file to be able
#        to extract translatable strings. .plugin is not recognized.
i18n.merge_file(
  input: 'exif-display.plugin.desktop.in',
  output: 'exif-display.plugin',
  po_dir: po_dir,
  type: 'desktop',
  args: ['--keyword=Name', '--keyword=Description'],
  install: true,
  install_dir: pluginlibdir,
)

configure_file(
  input: 'org.gnome.eog.plugins.exif-display.gschema.xml.in',
  output: '@BASENAME@',
  configuration: {'GETTEXT_PACKAGE': 'eog-plugins'},
  install: true,
  install_dir: gio_schemasdir,
)

exifdisplay_appdata = i18n.merge_file(
  input: 'eog-exif-display.appdata.xml.in',
  output: '@BASENAME@',
  po_dir: src_root / 'po',
  type: 'xml',
  install: true,
  install_dir: appstreamdir,
)

if appstream_util.found()
  test(
    'validate-eog-exif-display.appdata.xml',
    appstream_util,
    args: [
      'validate',
      '--nonet',
      exifdisplay_appdata.full_path(),
    ]
  )
endif

