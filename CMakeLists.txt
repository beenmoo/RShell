cmake_minimum_required(VERSION 3.21)

project(RShell LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Keep every target's output in one flat, predictable location regardless of which subdirectory
# defines it (otherwise multi-config generators like Visual Studio nest it per-subdirectory,
# e.g. Build/windows/Source/Debug/rshell.exe instead of Build/windows/Debug/rshell.exe).
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(Source)

option(RSHELL_BUILD_TESTS "Build RShell's test suite" ON)

if (RSHELL_BUILD_TESTS)
    enable_testing()
    add_subdirectory(Tests)
endif()
