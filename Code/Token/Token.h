#pragma once

#include <string>
#include <string_view>
#include <cstdint>

class TokenSpec
{
public:
    enum class TokenBaseType
    {
        None,
        Operand,
        Connector,
        Bracket
    };

    enum class TokenType
    {
        None,
        SingleCommand,
        And,
        Or,
        Semicolon,
        Exit,
        Test,
        LeftLegacyTest,
        RightLegacyTest,
        LeftParenthesis,
        RightParenthesis
    };

public:
    void SetTokenType(TokenSpec::TokenType type);
    TokenSpec::TokenType GetTokenType() const;
    TokenSpec::TokenBaseType GetTokenBaseType() const;

private:
    TokenType mTokenType = TokenType::None;
};

class Token
{
public:
    Token(std::string_view value = "");
    virtual ~Token() = default;

    void SetValue(std::string_view value);
    const std::string& GetValue() const;

    void SetTokenType(TokenSpec::TokenType type);

    const TokenSpec& GetSpec() const;

private:
    std::string mValue;

    TokenSpec mSpec;
};