#include "OrCommand.h"

bool OrCommand::Execute()
{
    return Left()->Execute() || Right()->Execute();
}
