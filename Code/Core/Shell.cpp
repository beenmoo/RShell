#include "Shell.h"
#include "Token/Token.h"
#include "Utils/Logging.h"

#include <iostream>

void Shell::Run()
{
    Logger::SetInstance(&mLogger);

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
    LOG_MESSAGE("$ ");
}

void Shell::GetInput()
{
    std::getline(std::cin, mInput);
}

void Shell::ProcessInput()
{
    const std::vector<Token> tokens = mLexer.TokenizeInput(mInput);
    const auto root = mParser.Parse(tokens);
    mExecutor.Execute(root);
}