#pragma once

#include "Token/Token.h"

#include <vector>

class Command
{
    using Arguments = std::vector<Token>;

public:
    using ConstIterator = Arguments::const_iterator;

    virtual ~Command() = default;

    virtual bool Execute() = 0;

    void SetArguments(const std::vector<Token>& args);
    void AddArgument(const Token& arg);

    size_t GetArgumentCount() const;

    const Token::TokenSpecification& GetTokenSpecification() const;

    ConstIterator begin() const;
    ConstIterator end() const;
    ConstIterator cbegin() const;
    ConstIterator cend() const;

protected:
    void SetTokenType(Token::TokenSpecification::TokenType type);

private:
    Token::TokenSpecification mSpecification;

    Arguments mArguments;
};