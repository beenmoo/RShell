#include "SemicolonCommand.h"

SemicolonCommand::SemicolonCommand()
{
    SetTokenType(TokenSpec::TokenType::Semicolon);
}

bool SemicolonCommand::Execute()
{
    if (!GetLeft() || !GetRight())
        return false;

    GetLeft()->Execute();

    return GetRight()->Execute();
}
