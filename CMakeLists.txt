
cmake_minimum_required(VERSION 3.1.0)

project(GLIB_EX C)

# Direct pkg-config to look in our debug version of glib:
set( glib_build /home/nmiller/build/glib )
set( ENV{PKG_CONFIG_PATH} ${glib_build}/lib/pkgconfig )

set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH ${glib_build}/lib)

# Find the 'glib' and 'gobject' utilities:
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB2 REQUIRED glib-2.0 gobject-2.0)

include_directories(${GLIB2_INCLUDE_DIRS})

add_executable(children children.c)
target_link_libraries(children ${GLIB2_LIBRARIES})

add_executable(interface interface.c)
target_link_libraries(interface ${GLIB2_LIBRARIES})

add_executable(hello hello.c)
