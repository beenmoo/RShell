#include "Shell.h"
#include "Token/Token.h"
#include "Utils/LoggingUtils.h"

#include <iostream>

void Shell::Run()
{
    while (true)
        Update();
}

void Shell::Update()
{
    PrintPrompt();
    GetInput();
    ProcessInput();
}

void Shell::PrintPrompt()
{
    std::cout << "$ ";
}

void Shell::GetInput()
{
    std::getline(std::cin, mInput);
}

void Shell::ProcessInput()
{
    LOG_MESSAGE("Input: " + mInput + "\n", Logging::LogType::None);

    std::vector<Token> tokens = mLexer.TokenizeInput(mInput);
    auto root = mParser.Parse(tokens);
    mExecutor.Execute(root);
}
