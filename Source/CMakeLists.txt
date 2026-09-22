add_library(rshell_core STATIC
    Command/AndCommand.cpp
    Command/CompositeCommand.cpp
    Command/ExitCommand.cpp
    Command/OrCommand.cpp
    Command/SemicolonCommand.cpp
    Command/SingleCommand.cpp
    Command/TestCommand.cpp
    Core/Executor.cpp
    Core/Lexer.cpp
    Core/Parser.cpp
    Core/Shell.cpp
    Platform/Process.cpp
    Token/Token.cpp
)

target_include_directories(rshell_core PUBLIC .)

if (MSVC)
    target_compile_options(rshell_core PUBLIC /W4)
else()
    target_compile_options(rshell_core PUBLIC -Wall -Wextra -Wpedantic)
endif()

add_executable(rshell main.cpp)
target_link_libraries(rshell PRIVATE rshell_core)
