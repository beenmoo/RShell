#include "Token.h"

Token::Token(std::string_view value)
    : mValue(value)
{
}

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
