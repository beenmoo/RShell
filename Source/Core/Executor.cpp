#include "Executor.h"

bool Executor::Execute(const std::shared_ptr<Command>& root)
{
    return root->Execute();
}
