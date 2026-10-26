cmake_minimum_required(VERSION 3.16)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
endif()

# no VERSION here: the git tag is the version (see docs/releasing.md)
project(soundbored DESCRIPTION "Free to use soundboard for linux & windows"
                   LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

# C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# C
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# MSVC: link the C runtime statically, so users don't need the VC++ redistributable.
# Set before FetchContent so dependencies use it too (mixing runtimes fails to link)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
# raylib asks for CMake 3.5, which ignores the line above (policy CMP0091). Force it on
set(CMAKE_POLICY_DEFAULT_CMP0091 NEW)

# raylib
set(CUSTOMIZE_BUILD ON CACHE BOOL "" FORCE)
set(SUPPORT_MODULE_RAUDIO OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5    # from clay's example. might change later
    GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(raylib)

# miniaudio (header-only)
FetchContent_Declare(miniaudio
    GIT_REPOSITORY https://github.com/mackron/miniaudio.git
    GIT_TAG 0.11.21
    GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(miniaudio)

# cord (header-only)
FetchContent_Declare(cord
    GIT_REPOSITORY https://github.com/louisbgl/cord-cpp.git
    GIT_TAG v1.0.1
    GIT_SHALLOW TRUE
    )
FetchContent_MakeAvailable(cord)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/*.cpp
    ${CMAKE_SOURCE_DIR}/src/*.c
)

add_executable(${PROJECT_NAME}
               ${SOURCES}
               ${CMAKE_SOURCE_DIR}/vendor/clay/clay_renderer_raylib.c)

target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
target_link_libraries(${PROJECT_NAME} PRIVATE cord)

target_include_directories(${PROJECT_NAME} PRIVATE
                           ${CMAKE_SOURCE_DIR}/src
                           ${CMAKE_SOURCE_DIR}/vendor/clay/
                           ${miniaudio_SOURCE_DIR})

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}
)

# Link platform libs for miniaudio
if(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE pthread m dl)
endif()

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE winmm ole32)
    # UTF-8 process codepage, so narrow file APIs (fopen) accept UTF-8 paths
    target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/resources/soundbored.manifest)
    # Release: GUI app, no console window. Keeps main() as the entry point instead of WinMain
    if(MSVC)
        target_link_options(${PROJECT_NAME} PRIVATE "$<$<CONFIG:Release>:/SUBSYSTEM:WINDOWS;/ENTRY:mainCRTStartup>")
    endif()
endif()
