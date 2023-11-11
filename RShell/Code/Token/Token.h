#pragma once

#include <string>
#include <string_view>
#include <cstdint>

class Token
{
public:
    class TokenSpecification
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
        void SetTokenType(TokenSpecification::TokenType type);
        TokenSpecification::TokenType GetTokenType() const;
        TokenSpecification::TokenBaseType GetTokenBaseType() const;

    private:
        TokenType mTokenType = TokenType::None;
    };

public:
    Token(std::string_view value = "");
    virtual ~Token() = default;

    void SetValue(std::string_view value);
    const std::string& GetValue() const;

    void SetTokenType(TokenSpecification::TokenType type);

    const TokenSpecification& GetSpecification() const;

private:
    std::string mValue;

    TokenSpecification mSpecification;
};