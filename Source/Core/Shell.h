#pragma once

#include "Executor.h"
#include "Lexer.h"
#include "Parser.h"

#include <string>

class Shell
{
public:
    // Runs until stdin reaches EOF, then returns whether the last command executed succeeded
    // (unaffected by blank lines or syntax errors, matching a real shell's $?).
    bool Run();

private:
    bool Update();

    void PrintPrompt();
    bool GetInput();
    void ProcessInput();

    std::string mInput;
    bool mLastResult = true;

    Executor mExecutor;
    Lexer mLexer;
    Parser mParser;
};