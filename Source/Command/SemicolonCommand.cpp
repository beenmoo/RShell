#include "SemicolonCommand.h"

bool SemicolonCommand::Execute()
{
    Left()->Execute();

    return Right()->Execute();
}
