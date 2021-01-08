Build Instructions
------------------

To configure and build:

  > meson build && meson compile -C build

To specify additional pkgconfig locations:

  > meson build --pkg-config-path ~/build/meson-glib/lib/pkgconfig/
OR
  > meson build --pkg-config-path ~/src/glib/build/meson-uninstalled

NOTE: This also sets the RPATH appropriately for us.

By default meson produces debug builds.  To build for release:

  > meson build --buildtype release

