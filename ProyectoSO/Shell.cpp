

// Shell.cpp
#include "Shell.h"

Shell::Shell() {
    commandHistory.clear();
    actualHistorySize = 0;
}

int Shell::executePipe(vector<string> firstCommand, vector<string> secondCommand) {
    if (firstCommand.empty() || secondCommand.empty()) {
        cerr << RED << "Error: La sintaxis del pipe es incorrecta." << RESET<< endl;
        return 0;
    }

    int pipeFd[2];
    if (pipe(pipeFd) == -1) {
        cerr << RED << "Error al crear la pipe" << RESET << endl;
        return 0;
    }

    executeProcess(firstCommand, STDIN_FILENO, pipeFd[1]);
    close(pipeFd[1]);

    executeProcess(secondCommand, pipeFd[0], STDOUT_FILENO);
    close(pipeFd[0]);

    return 1;
}
void Shell::executeProcess(vector<string>& command, int inputFd, int outputFd)
{
    pid_t pid = fork();

    if (pid == -1)
    {
        cerr << RESET << "Error al crear el proceso hijo" << RESET << endl;
        return;
    }
    if (pid == 0)
    {
        if (inputFd != STDIN_FILENO)
        {
            dup2(inputFd, STDIN_FILENO);
            close(inputFd);
        }
        if (outputFd != STDOUT_FILENO)
        {
            dup2(outputFd, STDOUT_FILENO);
            close(outputFd);
        }

        vector<const char*> argsPointers;
        for (auto& word : command)
        {
            argsPointers.push_back(word.c_str());
        }
        argsPointers.push_back(NULL);

        execvp(argsPointers[0], const_cast<char**>(argsPointers.data()));
        cerr << RED << "Error al ejecutar el comando: " << RESET << command[0] << endl;
        exit(1);
    }
    else
    {
        waitpid(pid, NULL, 0);
    }
}

void Shell::addCommand(const string& input){

    if(commandHistory.empty()){
        commandHistory.push_back(CommandRecord(1, input, 0));
    }else{
        commandHistory.push_back(CommandRecord(commandHistory.back().getPosition() + 1, input, 0));
    }
    actualHistorySize ++;
}

void Shell::deleteOldestCommand(){
     commandHistory.erase(commandHistory.begin());
}

void Shell::showCommandHistory()
{
    if(commandHistory.size() > 0){
        for(CommandRecord i : commandHistory){
            cout<< YELLOW << i.getPosition() << " -> " << i.getCommand() << " " << RESET <<endl;
        }
    }else{
        cout<< YELLOW << "HIstorial vacio" << RESET <<endl;
    }
}

string Shell::getCommand(int position){
    for(int  i = 0 ; i < commandHistory.size(); i++){
        if(commandHistory[i].getPosition() == position){
            return commandHistory[i].getCommand();
        }
    }
    return "";
}



