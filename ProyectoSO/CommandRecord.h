
#ifndef COMMANDRECORD_H
#define COMMANDRECORD_H
#include <string>
using namespace std;

class CommandRecord{
private:
    int position;
    string command;
    int timesRepeated;
public:
    CommandRecord();
    CommandRecord(int position, string command, int timesRepeated);
    int getPosition();
    string getCommand();
    int getTimesRepeated();
    void setPosition(int position);
    void setComand(string command);
    void setTimesRepeated(int timesRepeated);
};

#endif // COMMANDRECORD_H
