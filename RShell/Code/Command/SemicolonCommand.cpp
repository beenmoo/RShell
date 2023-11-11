#include "SemicolonCommand.h"

SemicolonCommand::SemicolonCommand()
{
    SetTokenType(Token::TokenSpecification::TokenType::Semicolon);
}

bool SemicolonCommand::Execute()
{
    GetLeftCommand()->Execute();

    return GetRightCommand()->Execute();
}
