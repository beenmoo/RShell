#include "Executor.h"
#include "Command/Command.h"

void Executor::Execute(const Ref<Command>& root)
{
    if (root)
        root->Execute();
}