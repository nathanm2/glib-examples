
# Turn all warnings into errors:
macro(warnings_into_errors)
if( CMAKE_COMPILER_IS_GNUCC )
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")
endif()

if( CMAKE_COMPILER_IS_GNUCXX )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
endif()

if( MSVC )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3 /WX")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W3 /WX")
endif()
endmacro()


# If this is the top-level project, output built targets to 'bin' or 'lib'.
#
# When searching for a DLL, the Windows loader only looks in a handful of spots,
# one of those being the directory containing the executable file.  In order to
# build DLLs and have the loader find them when running a unit-test we need to
# deposit everything into a common directory.
#
# For consistency across the various platforms we decided to do this for Linux
# as well, even though CMake can leverage the ELF's RPATH capabilities.
#
macro(project_output_dirs)
if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)

    # Setting these causes MSVC to NOT add a configuration prefix:
    # (e.g. .../Debug/foo.exe, .../Release/bar.dll)
    foreach(cfg "DEBUG" "RELEASE" "MINSIZEREL" "RELWITHDEBINFO")
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${cfg}
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${cfg}
            ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${cfg}
            ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
    endforeach()
endif()
endmacro()

macro(setup_conan)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake OPTIONAL)

if(COMMAND conan_basic_setup)
    conan_basic_setup(TARGETS NO_OUTPUT_DIRS)
endif()

endmacro()

#.rst
# select_target
# -------------
#
# Takes a list of potential targets and returns the first valid one.
#
# select_target(<variable> [REQUIRED]
#               [[TARGETS <target>...]
#                [MODULE <package> <target>...]
#                [CONFIG <package> <target>...]])
#
# If ``REQUIRED`` is specified the function will display an error and cause
# CMake to skip generation if a valid target is not found.
#
# All targets are tested in order until the first valid target is found.
#
# The targets following TARGETS are simply tested.
#
# The ``MODULE`` and ``CONFIG`` directives cause the function to first load a
# package via ``find_package(<package> QUIET [MODULE|CONFIG])`` and then test
# for valid targets.
#
# The ``TARGETS``, ``MODULE``, and ``CONFIG`` directives can be specified
# multiple times and in any order.

function(select_target out)
    set(required FALSE)
    set(start 1)
    set(mode "NONE")
    math(EXPR stop "${ARGC}-1")
    set(${out} NOTFOUND PARENT_SCOPE)
    set(out_target NOTFOUND)

    if(${ARGC} GREATER 1)
        if(${ARGV1} STREQUAL "REQUIRED")
            set(required true)
            set(start 2)
        endif()
    endif()

    foreach(idx RANGE ${start} ${stop})
        set(argv "${ARGV${idx}}")
        if("${ARGV${idx}}" MATCHES "TARGETS|MODULE|CONFIG")
            set(mode ${argv})
            set(package FALSE)
        else()
            if("${mode}" STREQUAL "TARGETS")
                if(TARGET ${ARGV${idx}})
                    message(STATUS "${out} found: TARGETS ${ARGV${idx}}")
                    set(out_target ${ARGV${idx}})
                    break()
                endif()
            elseif(NOT package)
                set(package ${ARGV${idx}})
            else()
                find_package(${package} QUIET ${mode})
                if(TARGET ${ARGV${idx}})
                    message(STATUS "${out} found: ${mode} ${package} ${ARGV${idx}}")
                    set(out_target ${ARGV${idx}})
                    break()
                endif()
            endif()
        endif()
    endforeach()

    set(${out} ${out_target} PARENT_SCOPE)
    if(required AND NOT TARGET ${out_target})
        message(SEND_ERROR "${out} not found.")
    endif()

endfunction(select_target)
