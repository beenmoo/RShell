#include "Shell.h"
#include "Token/Token.h"

#include <iostream>

bool Shell::Run()
{
    while (Update())
    {
    }

    return mLastResult;
}

bool Shell::Update()
{
    PrintPrompt();

    if (!GetInput())
        return false;

    ProcessInput();

    return true;
}

void Shell::PrintPrompt()
{
    std::cout << "$ ";
}

bool Shell::GetInput()
{
    return static_cast<bool>(std::getline(std::cin, mInput));
}

void Shell::ProcessInput()
{
    const std::vector<Token> tokens = mLexer.TokenizeInput(mInput);
    const auto root = mParser.Parse(tokens);

    if (root)
        mLastResult = mExecutor.Execute(root);
}