project('glib-examples', 'c')

glib = dependency('glib-2.0')
gobject = dependency('gobject-2.0')

executable('children', 'children.c', dependencies: [glib, gobject])
executable('gnew', 'gnew.c', dependencies: glib)
executable('gvariant', 'gvariant.c', dependencies : glib )
executable('deadman', 'deadman.c', dependencies: glib)

private_srcs = files(
  'gobject-private/main.c',
  'gobject-private/my-manager.c',
  )
executable('private', private_srcs, dependencies: gobject)
