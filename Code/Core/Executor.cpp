#include "Executor.h"
#include "Command/Command.h"
#include "Command/ConnectorCommand.h"

#include <stack>

void Executor::Execute(std::vector<Ref<Command>> postfix)
{
    std::stack<Ref<Command>> result;

    for (auto& cmd : postfix)
    {
        if (cmd->GetSpec().GetTokenBaseType() != TokenSpec::TokenBaseType::Operand)
        {
            auto& rightCmd = result.top();
            result.pop();

            auto& leftCmd = result.top();
            result.pop();

            auto connector = std::dynamic_pointer_cast<ConnectorCommand>(cmd);
            connector->SetLeft(leftCmd);
            connector->SetRight(rightCmd);

            result.emplace(connector);
        }
        else
            result.emplace(cmd);
    }

    if (!result.empty())
        result.top()->Execute();
}