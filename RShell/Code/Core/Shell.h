#pragma once

#include "Executor.h"
#include "Lexer.h"
#include "Parser.h"

#include <string>

class Shell
{
public:
    void Run();

private:
    void Update();

    void PrintPrompt();
    void GetInput();
    void ProcessInput();

private:
    std::string mInput;

    Executor mExecutor;
    Lexer mLexer;
    Parser mParser;
};