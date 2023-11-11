#include "Executor.h"
#include "Command/Command.h"
#include "Command/ConnectorCommand.h"

#include <stack>

void Executor::Execute(std::vector<std::shared_ptr<Command>> postfix)
{
    std::stack<std::shared_ptr<Command>> result;

    for (auto& cmd : postfix)
    {
        if (cmd->GetTokenSpecification().GetTokenBaseType() != Token::TokenSpecification::TokenBaseType::Operand)
        {
            auto& rightCmd = result.top();
            result.pop();

            auto& leftCmd = result.top();
            result.pop();

            auto connector = std::dynamic_pointer_cast<ConnectorCommand>(cmd);
            connector->SetLeftCommand(leftCmd);
            connector->SetRightCommand(rightCmd);

            result.emplace(connector);
        }
        else
            result.emplace(cmd);
    }

    if (!result.empty())
        result.top()->Execute();
}