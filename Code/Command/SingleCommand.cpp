#include "SingleCommand.h"

#include "Platform/Process.h"

#include <utility>

SingleCommand::SingleCommand(std::vector<std::string> args)
    : mArgs(std::move(args))
{
}

bool SingleCommand::Execute()
{
    return Platform::RunProcess(mArgs);
}
