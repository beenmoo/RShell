    find_package(GTest CONFIG REQUIRED)

    add_executable(rshell_tests
        Command/CompositeCommandTests.cpp
        Command/TestCommandTests.cpp
        Core/LexerTests.cpp
        Core/ParserTests.cpp
        Platform/ProcessTests.cpp
    )

    target_include_directories(rshell_tests PRIVATE .)
    target_link_libraries(rshell_tests PRIVATE rshell_core GTest::gtest GTest::gtest_main)

    include(GoogleTest)
    gtest_discover_tests(rshell_tests)