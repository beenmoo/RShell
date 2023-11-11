#include "SingleCommand.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>

SingleCommand::SingleCommand()
{
    SetTokenType(Token::TokenSpecification::TokenType::SingleCommand);
}

bool SingleCommand::Execute()
{
    std::vector<char*> args;

    for (const auto& token : *this)
        args.emplace_back(const_cast<char*>(token.GetValue().c_str()));

    args.emplace_back(nullptr);

    pid_t pid = fork();

    if (pid > 0)
    {
        int status;

        if (waitpid(pid, &status, 0) == -1)
        {
            perror("wait");

            return false;
        }

        if (WEXITSTATUS(status))
            return false;
    }
    else if (!pid)
    {
        if (execvp(args[0], args.data()) == -1)
        {
            std::cout << "Shell: " << args[0] << ": " << "command not found" << std::endl;

            exit(EXIT_FAILURE);
        }
    }
    else
    {
        perror("fork");

        return false;
    }

    return true;
}