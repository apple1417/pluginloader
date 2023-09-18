cmake_minimum_required(VERSION 3.24)

project(pluginloader VERSION 1.0.0)

add_library(_pluginloader_base INTERFACE)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

target_compile_features(_pluginloader_base INTERFACE cxx_std_20)
set_target_properties(_pluginloader_base PROPERTIES
    COMPILE_WARNING_AS_ERROR True
    INTERPROCEDURAL_OPTIMIZATION True
    PREFIX ""
)
if(MSVC)
    target_compile_options(_pluginloader_base INTERFACE /W4)
else()
    target_compile_options(_pluginloader_base INTERFACE -Wall -Wextra -Wpedantic)
endif()

set(CONFIGURE_FILES_DIR "${CMAKE_CURRENT_BINARY_DIR}/configure")

configure_file(
    "src/version.inl.in"
    "${CONFIGURE_FILES_DIR}/version.inl"
)

set(GIT_PRE_CONFIGURE_FILE "src/git.inl.in")
set(GIT_POST_CONFIGURE_FILE "${CONFIGURE_FILES_DIR}/git.inl")
include(common_cmake/git_watcher.cmake)

# Note not recursive, only top level - we specify proxy explicitly
file(GLOB sources CONFIGURE_DEPENDS "src/*.cpp" "src/*.h")
target_sources(_pluginloader_base PUBLIC
    ${sources}
    ${GIT_POST_CONFIGURE_FILE}
    "src/proxy/proxy.h"
    "src/versioninfo.rc"
)
target_include_directories(_pluginloader_base INTERFACE "src" ${CONFIGURE_FILES_DIR})
target_precompile_headers(_pluginloader_base INTERFACE "src/pch.h")

# ==================================================================================================

add_library(pluginloader_no_proxy SHARED "src/proxy/none.cpp")
target_link_libraries(pluginloader_no_proxy PUBLIC _pluginloader_base)

install(
    TARGETS
        pluginloader_no_proxy
    RUNTIME DESTINATION .
)
