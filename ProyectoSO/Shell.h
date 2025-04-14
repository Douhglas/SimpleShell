// Shell.h
#ifndef SHELL_H
#define SHELL_H

#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"

#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "CommandRecord.h"

using namespace std;

class Shell {
public:
    int actualHistorySize;
private:
    vector<CommandRecord> commandHistory;
    string command;

public:
    Shell();
    int executePipe(vector<string> firstCommand, vector<string> secondCommand);
    void executeProcess(vector<string>& command, int inputFd = STDIN_FILENO, int outputFd = STDOUT_FILENO);
    void addCommand(const string& input);
    void deleteOldestCommand();
    void showCommandHistory();
    string getCommand(int position);
};

#endif // SHELL_H
