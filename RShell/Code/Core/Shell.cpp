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
    Utils::Logging::LogMessage("Input: " + mInput + "\n");

    std::vector<Token> tokens = mLexer.TokenizeInput(mInput);
    auto postfix = mParser.Parse(tokens);
    mExecutor.Execute(postfix);
}
