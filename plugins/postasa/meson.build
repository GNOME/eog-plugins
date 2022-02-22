postasa_sources = files(
  'eog-postasa-plugin.c',
)

postasa_res = gnome.compile_resources(
  'eog-postasa-resources',
  'eog-postasa-plugin.gresource.xml',
)

postasa_sources += [ postasa_res.get(0) ]
libgdata_dep = dependency('libgdata', version: '>= 0.6.0')

postasa_deps = [
  eog_dep,
  libpeas_dep,
  libpeasgtk_dep,
  libgdata_dep,
]

config_h.set('HAVE_LIBGDATA_0_8',
             libgdata_dep.version().version_compare('>= 0.8'),
             description: 'Define to be compatible with the API of libgdata-0.8')
config_h.set('HAVE_LIBGDATA_0_9',
             libgdata_dep.version().version_compare('>= 0.9'),
             description: 'Define to be compatible with the API of libgdata-0.9')

postasa_plugin = shared_module(
  'postasa',
  sources: postasa_sources,
  include_directories: root_include_dir,
  dependencies: postasa_deps,
  install: true,
  install_dir: pluginlibdir,
)

# FIXME: Remove `args` parameter when gettext acquires plugin support.
#        http://lists.gnu.org/archive/html/bug-gettext/2017-06/msg00001.html
# NOTE:  We need to keep the .desktop suffix on the source file to be able
#        to extract translatable strings. .plugin is not recognized.
i18n.merge_file(
  input: 'postasa.plugin.desktop.in',
  output: 'postasa.plugin',
  po_dir: po_dir,
  type: 'desktop',
  args: ['--keyword=Name', '--keyword=Description'],
  install: true,
  install_dir: pluginlibdir,
)

postasa_appdata = i18n.merge_file(
  input: 'eog-postasa.appdata.xml.in',
  output: '@BASENAME@',
  po_dir: src_root / 'po',
  type: 'xml',
  install: true,
  install_dir: appstreamdir,
)

if appstream_util.found()
  test(
    'validate-eog-postasa.appdata.xml',
    appstream_util,
    args: [
      'validate',
      '--nonet',
      postasa_appdata.full_path(),
    ]
  )
endif

