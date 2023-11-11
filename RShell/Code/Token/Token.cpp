#include "Token.h"

Token::Token(std::string_view value) :
    mValue(value)
{}

void Token::SetValue(std::string_view value)
{
    mValue = value;
}

const std::string& Token::GetValue() const
{
    return mValue;
}

void Token::SetTokenType(TokenSpecification::TokenType type)
{
    mSpecification.SetTokenType(type);
}

const Token::TokenSpecification& Token::GetSpecification() const
{
    return mSpecification;
}

void Token::TokenSpecification::SetTokenType(TokenSpecification::TokenType type)
{
    mTokenType = type;
}

Token::TokenSpecification::TokenType Token::TokenSpecification::GetTokenType() const
{
    return mTokenType;
}

Token::TokenSpecification::TokenBaseType Token::TokenSpecification::GetTokenBaseType() const
{
    switch (mTokenType)
    {
    case TokenType::SingleCommand:
    case TokenType::Exit:
    case TokenType::Test:
        return TokenBaseType::Operand;
    case TokenType::Or:
    case TokenType::And:
    case TokenType::Semicolon:
        return TokenBaseType::Connector;
    case TokenType::LeftLegacyTest:
    case TokenType::RightLegacyTest:
    case TokenType::LeftParenthesis:
    case TokenType::RightParenthesis:
        return TokenBaseType::Bracket;
    default:
        break;
    }

    return TokenBaseType::None;
}
