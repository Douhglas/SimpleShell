#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <regex>
#include <readline/readline.h>
#include <readline/history.h>
#include "Shell.h"

using namespace std;

#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"

vector<string> firstCommand;// Almacena el comando principal
vector<string> secondCommand;// Almacena el segundo comando si hay una pipe
bool isBackgroundProcess = false;// Indica si el proceso se ejecuta en segundo plano
bool hasPipe = false;// Indica si hay una pipe en el comando
Shell shell;


 //Separa el input del usuario en comandos y detecta si hay una pipe.
void parseUserInput(string userInput) {
    firstCommand.clear();
    secondCommand.clear();
    hasPipe = false;

    istringstream inputStream(userInput);
    string word;

    while (inputStream >> word) {
        if (word == "|") {
            hasPipe = true;
            continue;
        }
        if (hasPipe) {
            secondCommand.push_back(word);
        } else {
            firstCommand.push_back(word);
        }
    }
}
void addCommand(char*& inputChar, string userInput){
        add_history(inputChar);
        shell.addCommand(userInput);
        if(shell.actualHistorySize > 10){
            shell.deleteOldestCommand();
        }
}
void specialCommand(string& userInput){
    regex pattern("!(\\d+)");
    smatch match;
    while(regex_search(userInput,match,pattern)){
        int commandPosition = stoi(match[1]);
        string command = shell.getCommand(commandPosition);
        if(command.empty()){
            cout<< YELLOW << "No se encontro el comando en la posicion " << RESET << commandPosition << YELLOW << " del historial" << RESET<<endl;
            return;
        }else{
          userInput = regex_replace(userInput,pattern,command);
        }
    }
}

int main() {
using_history();
stifle_history(10);
    while (true) { //bucle principal
        isBackgroundProcess = false;
        hasPipe = false;

        char* inputChar = readline((GREEN + string("Ahab> ") + RESET).c_str());
        string userInput(inputChar);
        if (userInput == "exit") break;
        specialCommand(userInput); // Revisa si hay un !n, y lo reemplaza por el comando correspondiente 
        inputChar = new char[userInput.size() + 1];
        strcpy(inputChar,userInput.c_str());
        addCommand(inputChar, userInput);//Agrega el comando al historial del shell y del readline
        free(inputChar);

        if (userInput.empty()) continue;
        if (userInput == "historial") {
           shell.showCommandHistory();
           continue;
        };
        parseUserInput(userInput);

        if (!firstCommand.empty() && firstCommand.back() == "&") {
            firstCommand.pop_back();
            isBackgroundProcess = true;
        }
        if (!firstCommand.empty()) {
            if (firstCommand[0] == "cd") {
                if (firstCommand.size() > 1) {
                    if (chdir(firstCommand[1].c_str()) != 0) {
                        perror("cd");
                    }
                } else {
                    const char* home = getenv("HOME");
                    if (home) chdir(home);
                }
                continue;
            }
            if (hasPipe) {
                if (shell.executePipe(firstCommand, secondCommand) == 0) continue;
            } else {
                shell.executeProcess(firstCommand);
            }
        }
    }

    cout << "Fin" << endl;
    return 0;
}


