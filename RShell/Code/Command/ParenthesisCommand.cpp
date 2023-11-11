#include "ParenthesisCommand.h"

LeftParenthesisCommand::LeftParenthesisCommand()
{
    SetTokenType(Token::TokenSpecification::TokenType::LeftParenthesis);
}

bool LeftParenthesisCommand::Execute()
{
    return false;
}

RightParenthesisCommand::RightParenthesisCommand()
{
    SetTokenType(Token::TokenSpecification::TokenType::RightParenthesis);
}

bool RightParenthesisCommand::Execute()
{
    return false;
}
