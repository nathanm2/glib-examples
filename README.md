Build Instructions
------------------

Run meson and ninja:

  > meson build
  > ninja -C build

To use a local build of GLib specify the `PKG_CONFIG_PATH` environment
variable:

  > PKG_CONFIG_PATH=~/build/meson-glib/lib/pkgconfig/ meson build
  > ninja -v -C build

NOTE: This also sets the RPATH appropriately for us.

By default meson produces debug builds.  To build for release:

  > meson --buildtype release build

