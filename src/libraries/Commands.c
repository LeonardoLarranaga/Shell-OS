#include "Commands.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define READ_END 0
#define WRITE_END 1

// mycat - Concatena y muestra el contenido de uno o más archivos.
void mycat(char** arguments, int argumentCount) {
    if (argumentCount == 0) {
        printf("mycat: Expected at least one argument.\n");
        return;
    }

    int fd;
    // Iterar sobre los argumentos y mostrar su contenido
    for (int i = 0; i < argumentCount; i++) {
        char* result = NULL;
        readFile(&result, arguments[i], &fd);
        
        if (fd == -1) 
            printf("mycat: '%s' not found or you don't have permission to open it.\n", arguments[i]);
        else {
            printf("%s\n", result);
            free(result);
            close(fd);
        }
    }
}

// type - Crea un archivo con el contenido ingresado por el usuario.
void type(char** arguments, int argumentCount) {
    if (argumentCount == 0) {
        printf("type: Expected at least one argument.\n");
        return;
    } else if (argumentCount > 1) {
        printf("type: Expected only one argument.\n");
        return;
    }

    // Crear o abrir el archivo
    int fd = createTypeFile(arguments[0]);
    if (fd == -1) {
        printf("type: '%s' couldn't be created or opened.\n", arguments[0]);
        return;
    }

    char* fullString = (char*) malloc(1);

    // Leer el contenido ingresado por el usuario
    while (1) {
        char* line = readLine();
        // Si el usuario ingresa '$', terminar la lectura
        if (strchr(line, '$') != NULL) {
            *strchr(line, '$') = '\0';
            fullString = (char*) realloc(fullString, strlen(fullString) + strlen(line) + 5);
            strcat(fullString, line);
            break;
        }

        fullString = (char*) realloc(fullString, strlen(fullString) + strlen(line) + 5);
        strcat(fullString, line);
        strcat(fullString, "\n");
    }

    // Escribir el contenido en el archivo
    int writeStatus = writeTypeFile(fd, fullString);
    if (writeStatus == -1) printf("type: '%s' couldn't be written.", arguments[0]);
    close(fd);
    free(fullString);
}

// mycp - Copia el contenido de un archivo a uno o más archivos.
void mycp(char** arguments, int argumentCount) {
    if (argumentCount < 2) {
        printf("mycp: Expected at least two arguments.\n");
        return;
    }

    int fd;
    char* result = NULL;
    readFile(&result, arguments[0], &fd);

    if (fd == -1) {
        printf("mycp: '%s' not found or you don't have permission to open it.\n", arguments[0]);
        return;
    } 

    // Iterar sobre los argumentos y copiar el contenido
    for (int i = 1; i < argumentCount; i++) {
        fd = createTypeFile(arguments[i]);
        if (fd == -1) {
            printf("mycp: '%s' couldn't be created or opened.\n", arguments[0]);
            continue;;
        }

        int writeStatus = writeTypeFile(fd, result);
        if (writeStatus == -1) printf("mycp: '%s' couldn't be written.", arguments[0]);
        close(fd);
    }
    
    free(result);
}

// myremove - Elimina uno o más archivos.
void myremove(char** arguments, int argumentCount) {
    if (argumentCount == 0) {
        printf("remove: Expected at least one argument.\n");
        return;
    }

    // Iterar sobre los argumentos y eliminar los archivos
    for (int i = 0; i < argumentCount; i++) {
        int status = removeFile(arguments[i]);
        if (status == -1) printf("remove: '%s' couldn't be removed.\n", arguments[i]);
    }
}

// parseCommands - Parsea los comandos ingresados por el usuario
char** parseCommands(char* command, const char* delimiter) {
    int count = 0;

    // Contar la cantidad de comandos
    char* token = strtok(strdup(command), delimiter);
    while (token != NULL) {
        count += 1;
        token = strtok(NULL, delimiter);
    }

    // Crear un arreglo de comandos
    char** commands = malloc((count + 1) * sizeof(char*));
    token = strtok(strdup(command), delimiter);

    int i = 0;
    // Iterar sobre los comandos y agregarlos al arreglo
    for (i = 0; token != NULL; i++) {
        while (*token == ' ') token += 1;
        commands[i] = strdup(token);
        token = strtok(NULL, delimiter);
    }
    commands[i] = NULL;

    return commands;
}

// executeCommands - Ejecuta los comandos ingresados por el usuario
void executeCommands(char** commands) {
    // Contar la cantidad de comandos
    int commandsAmount = 0;
    while (commands[commandsAmount] != NULL) commandsAmount++;

    // Crear pipes para la comunicación entre procesos
    int pipesAmount = commandsAmount - 1;
    int pipes[pipesAmount][2];

    // Crear los pipes
    for (int i = 0; i < pipesAmount; i++) {
        if (pipe(pipes[i]) == -1) {
            fprintf(stderr, "Pipe %d failed\n", i);
            return;
        }
    }

    // Crear procesos hijos para ejecutar los comandos
    for (int i = 0; i < commandsAmount; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            fprintf(stderr, "Fork failed\n");
            return;
        }

        // Redireccionar la entrada y salida estándar de los procesos
        if (pid == 0) { 
            if (i == 0) {
                dup2(pipes[i][WRITE_END], STDOUT_FILENO);
                close(pipes[i][READ_END]);
            } else if (i == pipesAmount) {
                dup2(pipes[i - 1][READ_END], STDIN_FILENO);
            } else {
                dup2(pipes[i - 1][READ_END], STDIN_FILENO);
                dup2(pipes[i][WRITE_END], STDOUT_FILENO);
            }

            // Cerrar los pipes
            for (int j = 0; j < pipesAmount; j++) {
                close(pipes[j][READ_END]);
                close(pipes[j][WRITE_END]);
            }

            // Ejecutar el comando
            char** arguments = parseCommands(commands[i], " ");
            execvp(arguments[0], arguments);
            free(arguments);
        }
    }

    // Cerrar los pipes
    for (int j = 0; j < pipesAmount; j++) {
            close(pipes[j][READ_END]);
            close(pipes[j][WRITE_END]);
    }

    // Esperar a que los procesos hijos terminen
    for (int i = 0; i < commandsAmount; i++) {
        wait(NULL);
    }
}