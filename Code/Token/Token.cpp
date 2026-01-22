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

void Token::SetTokenType(TokenSpec::TokenType type)
{
    mSpec.SetTokenType(type);
}

const TokenSpec& Token::GetSpec() const
{
    return mSpec;
}

void TokenSpec::SetTokenType(TokenSpec::TokenType type)
{
    mTokenType = type;
}

TokenSpec::TokenType TokenSpec::GetTokenType() const
{
    return mTokenType;
}

TokenSpec::TokenBaseType TokenSpec::GetTokenBaseType() const
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
