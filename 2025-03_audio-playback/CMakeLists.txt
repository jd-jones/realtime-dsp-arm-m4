# == PROJECT SETUP ============================================================
cmake_minimum_required(VERSION 3.21.0)

project(
    2025-03_audio-playback
    VERSION 0.1
    DESCRIPTION "STM32F4 project that plays back audio from a USB flash drive."
    LANGUAGES C ASM
)

set(CMAKE_SYSTEM_NAME               Generic)
set(CMAKE_SYSTEM_PROCESSOR          arm)
set(CMAKE_C_STANDARD                11)
set(CMAKE_C_STANDARD_REQUIRED       ON)
set(CMAKE_C_EXTENSIONS              ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS   ON)

set(cmake_root                      ./cmake)
set(docs_root                       ./docs)
set(tests_root                      ./tests)
set(app_root                        ./app)
# =============================================================================


# == CMAKE STUFF ==============================================================
include(${cmake_root}/cppcheck.cmake)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    find_package(Doxygen)
    if(Doxygen_FOUND)
      add_subdirectory(${docs_root})
    else()
      message(STATUS "Doxygen not found, not building docs")
    endif()
endif()

include(FetchContent)
include(CTest)
add_subdirectory(${tests_root})

add_subdirectory(${app_root})
# =============================================================================
