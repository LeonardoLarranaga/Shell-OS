#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Commands.h"

// Lista de procesos
processNode_t* processList = NULL;

// Lista de bloques de memoria
memoryBlock_t* memoryList = NULL;

// Tamaño de la memoria
int memorySize = 0;

// Macro para liberar memoria
#define FREEMEMORY freeMemory(input, command, arguments, argumentCount)

// Declaración de funciones
void freeMemory(char*, char*, char**, int);
void executeCommand(char*, char**, int);

char* input = NULL;

void main() {
    char* command = NULL;
    char** arguments = NULL;
    int argumentCount = 0;

    system("clear");
    while (1) {
        printf("kaly:~$ Enter memory size in bytes: ");
        input = readLine();
        memorySize = atoi(input);
        if(memorySize <= 0) 
            printf("kaly:~$ Expected memory size bigger than zero.\n");
        else {            
            memoryBlock_t memory = {
                .processName = "",       
                .address = 0,
                .status = FREE,
                .next = NULL,
                .blockSize = memorySize
            };
            
            memoryList = &memory;
    
            break;
        }
    }


    while (1) {
        printf("kaly:~$ ");
        input = readLine();

        // Parsear la entrada en comando y argumentos
        parseArguments(strdup(input), &command, &arguments, &argumentCount);
        if (command == NULL) {
            FREEMEMORY;
            continue;
        }

        // Salir del shell si el comando es "exit"
        if (!strcmp(command, "exit")) {
            FREEMEMORY;
            break;
        } else {
            // Ejecutar el comando
            executeCommand(command, arguments, argumentCount);
        }

        // Liberar memoria asignada
        FREEMEMORY;
    }
}

// Función para liberar memoria asignada
void freeMemory(char* input, char* command, char** arguments, int argumentCount) {
    free(input);
    free(command);
    for (int i = 0; i < argumentCount; i++) {
        free(arguments[i]);
    }
    free(arguments);
}

// Función para ejecutar un comando
void executeCommand(char* command, char** arguments, int argumentCount) {
    if (!strcmp(command, "mycat")) {
        mycat(arguments, argumentCount);
    } else if (!strcmp(command, "type")) {
        type(arguments, argumentCount);
    } else if (!strcmp(command, "mycp")) {
        mycp(arguments, argumentCount);
    } else if (!strcmp(command, "clear")) {
        system("clear");
    } else if (!strcmp(command, "remove")) {
        myremove(arguments, argumentCount);
    } else if (!strcmp(command, "mkprocess")) {
        mkprocess(arguments, argumentCount);
    } else if (!strcmp(command, "lsprocesses")) {
        lsprocesses();
    } else if (!strcmp(command, "fcfs")) {
        firstComeFirstServed("FCFS");
    } else if (!strcmp(command, "sjf")) {
        shortestJobFirst();
    } else if (!strcmp(command, "roundrobin")) {
        roundRobinMemory(arguments, argumentCount);
    } else if (!strcmp(command, "killprocess")) {
        killprocess(arguments, argumentCount);
    } else if (!strcmp(command, "allocate")) {
        allocate(arguments, argumentCount);
    } else if (!strcmp(command, "freeprocess")) {
        freeprocess(arguments, argumentCount);
    } else if (!strcmp(command, "memorystatus")) {
        memorystatus();
    } else if (!strcmp(command, "compactmemory")) {
        compactmemory();
    } else {
        // Manejar comandos con pipes
        char** commands = parseCommands(input, "|");
        executeCommands(commands);
        free(commands);
    }
}