#pragma once

#include <string>
#include <string_view>
#include <cstdint>

class TokenSpec
{
public:
    enum class TokenType : std::uint8_t
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

    void SetTokenType(TokenSpec::TokenType type);
    TokenSpec::TokenType GetTokenType() const;

private:
    TokenType mTokenType = TokenType::None;
};

class Token
{
public:
    Token(std::string_view value = "");
    ~Token() = default;

    void SetValue(std::string_view value);
    const std::string& GetValue() const;

    void SetTokenType(TokenSpec::TokenType type);

    const TokenSpec& GetSpec() const;

private:
    std::string mValue;

    TokenSpec mSpec;
};
