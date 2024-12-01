#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Terminal.h"
#include "Commands.h"

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

    while (1) {
        printf("yuniks:~$ ");
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
        fcfs();
    } else if (!strcmp(command, "sjf")) {
        sjf();
    } else if (!strcmp(command, "roundrobin")) {
        roundRobin(arguments, argumentCount);
    } else {
        // Manejar comandos con pipes
        char** commands = parseCommands(input, "|");
        executeCommands(commands);
        free(commands);
    }
}