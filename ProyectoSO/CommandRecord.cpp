#include "CommandRecord.h"

CommandRecord::CommandRecord()
{

}
CommandRecord::CommandRecord(int position, string command, int timesRepeated)
{
    this->position = position;
    this->command = command;
    this->timesRepeated = timesRepeated;
}

int CommandRecord::getPosition()
{
    return position;
}

string CommandRecord::getCommand()
{
    return command;
}

int CommandRecord::getTimesRepeated()
{
    return timesRepeated;
}
void CommandRecord::setPosition(int position)
{
    this->position = position;
}

void CommandRecord::setComand(string command)
{
    this->command = command;
}

void CommandRecord::setTimesRepeated(int timesRepeated)
{
    this->timesRepeated = timesRepeated;
}
