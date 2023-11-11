#pragma once

#include "Token/Token.h"

#include <string>
#include <vector>
#include <memory>
#include <stack>

class Command;

class Parser
{
    template<typename T>
    using Ref = std::shared_ptr<T>;

    template<typename T, typename... Args>
    constexpr Ref<T> CreateRef(Args&&... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

private:
    enum class ShuntingState
    {
        ExpectOperand,
        ExpectConnector,
        ExpectBracket
    };

    enum class ErrorState
    {
        None,
        OperandError,
        ConnectorError,
        BracketError
    };

public:
    std::vector<Ref<Command>> Parse(const std::vector<Token>& tokens);

private:
    void ConstructCommands();
    bool ConstructPostfix();
    bool ProcessConnectorsPostfix(const Ref<Command>& cmd);
    bool ProcessLeftBracket(const Ref<Command>& cmd);
    bool ProcessRightBracket(const Ref<Command>& cmd);

    std::shared_ptr<Command> CreateCommand(const Token& token,
                                           const std::vector<Token>& args = {});

    void LogError(ErrorState state) const;

    void Reset();

private:
    std::vector<Token> mTokens;
    std::vector<Ref<Command>> mInfixCommands;
    std::vector<Ref<Command>> mPostfixCommands;
    std::stack<Ref<Command>> mConnectors;
    ShuntingState mShuntingState = ShuntingState::ExpectOperand;
};