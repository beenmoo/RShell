#include "ParenthesisCommand.h"

LeftParenthesisCommand::LeftParenthesisCommand()
{
    SetTokenType(TokenSpec::TokenType::LeftParenthesis);
}

bool LeftParenthesisCommand::Execute()
{
    return false;
}

RightParenthesisCommand::RightParenthesisCommand()
{
    SetTokenType(TokenSpec::TokenType::RightParenthesis);
}

bool RightParenthesisCommand::Execute()
{
    return false;
}
