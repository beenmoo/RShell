#include "SemicolonCommand.h"

SemicolonCommand::SemicolonCommand()
{
    SetTokenType(Token::TokenSpecification::TokenType::Semicolon);
}

bool SemicolonCommand::Execute()
{
    GetLeft()->Execute();

    return GetRight()->Execute();
}
