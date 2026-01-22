#pragma once

#include "Token/Token.h"
#include "Utils/Ref.h"

#include <string>
#include <vector>
#include <memory>
#include <stack>

class Command;

class Parser
{
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

    void LogError(ErrorState state) const;

    void Reset();

private:
    std::vector<Token> mTokens;
    std::vector<Ref<Command>> mInfixCommands;
    std::vector<Ref<Command>> mPostfixCommands;
    std::stack<Ref<Command>> mConnectors;
    ShuntingState mShuntingState = ShuntingState::ExpectOperand;
};