#include "Command.h"

void Command::SetArguments(const std::vector<Token>& args)
{
    mArguments = args;
}

void Command::AddArgument(const Token& arg)
{
    mArguments.emplace_back(arg);
}

size_t Command::GetArgumentCount() const
{
    if (mArguments.empty())
        return 0;

    return mArguments.size() - 1;
}

const Token::TokenSpecification& Command::GetTokenSpecification() const
{
    return mSpecification;
}

Command::ConstIterator Command::begin() const
 {
     return mArguments.begin();
 }

 Command::ConstIterator Command::end() const
 {
     return mArguments.end();
 }

 Command::ConstIterator Command::cbegin() const
 {
     return mArguments.cbegin();
 }

 Command::ConstIterator Command::cend() const
 {
     return mArguments.cend();
 }

 void Command::SetTokenType(Token::TokenSpecification::TokenType type)
 {
     mSpecification.SetTokenType(type);
 }