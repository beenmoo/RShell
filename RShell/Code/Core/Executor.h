#pragma once

#include <vector>
#include <memory>

class Command;

class Executor
{
public:
    void Execute(std::vector<std::shared_ptr<Command>> postfix);
};