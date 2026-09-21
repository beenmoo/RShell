cmake_minimum_required(VERSION 3.21)

project(RShell LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(rshell
    Code/main.cpp
    Code/Command/AndCommand.cpp
    Code/Command/CompositeCommand.cpp
    Code/Command/ExitCommand.cpp
    Code/Command/OrCommand.cpp
    Code/Command/SemicolonCommand.cpp
    Code/Command/SingleCommand.cpp
    Code/Command/TestCommand.cpp
    Code/Core/Executor.cpp
    Code/Core/Lexer.cpp
    Code/Core/Parser.cpp
    Code/Core/Shell.cpp
    Code/Platform/Process.cpp
    Code/Token/Token.cpp
)

target_include_directories(rshell PRIVATE Code)

if (MSVC)
    target_compile_options(rshell PRIVATE /W4)
else()
    target_compile_options(rshell PRIVATE -Wall -Wextra -Wpedantic)
endif()
