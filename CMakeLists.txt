cmake_minimum_required(VERSION 3.24)

project(pluginloader VERSION 1.0.0)

add_library(pluginloader SHARED)

target_compile_features(pluginloader INTERFACE cxx_std_20)
set_target_properties(pluginloader PROPERTIES
    COMPILE_WARNING_AS_ERROR True
    INTERPROCEDURAL_OPTIMIZATION True
    EXPORT_COMPILE_COMMANDS True
    PREFIX ""
)
if(MSVC)
    target_compile_options(pluginloader PRIVATE /W4)
else()
    target_compile_options(pluginloader PRIVATE -Wall -Wextra -Wpedantic)
endif()

set(CONFIGURE_FILES_DIR "${CMAKE_CURRENT_BINARY_DIR}/configure")

configure_file(
    "src/version.inl.in"
    "${CONFIGURE_FILES_DIR}/version.inl"
)

set(GIT_PRE_CONFIGURE_FILE "src/git.inl.in")
set(GIT_POST_CONFIGURE_FILE "${CONFIGURE_FILES_DIR}/git.inl")
include(common_cmake/git_watcher.cmake)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "src/*.cpp" "src/*.h")
target_sources(pluginloader PUBLIC
    ${sources}
    ${GIT_POST_CONFIGURE_FILE}
    "src/versioninfo.rc"
)

target_include_directories(pluginloader PUBLIC "src" ${CONFIGURE_FILES_DIR})

target_precompile_headers(pluginloader PUBLIC "src/pch.h")

install(
    TARGETS pluginloader
    RUNTIME DESTINATION .
)
