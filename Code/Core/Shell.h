#pragma once

#include "Executor.h"
#include "Lexer.h"
#include "Parser.h"
#include "Utils/Logging.h"

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

    std::string mInput;

    Logger mLogger;
    Executor mExecutor;
    Lexer mLexer;
    Parser mParser;
};