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

    const TokenSpec& GetSpec() const;

    ConstIterator begin() const;
    ConstIterator end() const;
    ConstIterator cbegin() const;
    ConstIterator cend() const;

protected:
    void SetTokenType(TokenSpec::TokenType type);

private:
    TokenSpec mSpec;

    Arguments mArguments;
};