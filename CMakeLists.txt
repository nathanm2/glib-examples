
cmake_minimum_required(VERSION 3.6.3)

project(GLIB_EX C)

include(cmake/utils.cmake)

# Turn compiler warnings into errors:
warnings_into_errors()

# Output binaries to 'bin' or 'lib':
project_output_dirs()

# Setup conan:
setup_conan()

# Setup pkg-config:

# Direct pkg-config to look in our debug version of glib:
set( glib_build /home/nmiller/build/glib )
set( ENV{PKG_CONFIG_PATH} ${glib_build}/lib/pkgconfig )

# set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
# set(CMAKE_INSTALL_RPATH ${glib_build}/lib)

find_package(PkgConfig)

# Find the 'glib' and 'gobject' utilities:
select_target(glib REQUIRED
    TARGETS CONAN_PKG::glib
    PKG_CONFIG glib-2.0)

select_target(gobject REQUIRED
    TARGETS CONAN_PKG::glib
    PKG_CONFIG gobject-2.0)

add_executable(basic_glib basic_glib.c)
target_link_libraries(basic_glib glib)

add_executable(children children.c)
target_link_libraries(children gobject)

add_executable(interface interface.c)
target_link_libraries(interface gobject)
