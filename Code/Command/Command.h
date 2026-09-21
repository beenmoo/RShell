#pragma once

class Command
{
public:
    virtual ~Command() = default;

    virtual bool Execute() = 0;
};
