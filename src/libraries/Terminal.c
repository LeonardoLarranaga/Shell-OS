#include "Terminal.h"

#include <fcntl.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Leer una línea de la terminal sin conocer su longitud.
char* readLine() {
    char* input = NULL;
    char c;
    int length = 0;

    // Leer hasta encontrar un salto de línea o un fin de archivo.
    while ((c = getchar()) != '\n' && c != EOF) {
        // Reasignar memoria para la nueva letra.
        input = realloc(input, length + 1);
        input[length] = c;
        length += 1;
    }

    // Agregar un caracter nulo al final de la cadena.
    input = realloc(input, length + 1);
    input[length] = '\0';

    return input;
}

// Parsear los argumentos de una cadena de texto.
void parseArguments(char* input, char** command, char*** arguments, int* argumentCount) {
    char* token;
    *argumentCount = 0;
    *arguments = malloc(sizeof(char*));

    // Separar la cadena de texto por espacios, donde el primer token es el comando. 
    token = strtok(input, " ");
    if (token != NULL) 
        *command = strdup(token);
    else {
        *command = NULL;
        return;
    }

    bool inQuotes = false;
    char* tempArgument = NULL;

    // Separar los argumentos de la cadena de texto.
    while ((token = strtok(NULL, " ")) != NULL) {
        // Manejar paths completos...
        if (token[0] == '\'' || token[0] == '\"' || token[strlen(token) - 1] == '\'' || token[strlen(token) - 1] == '\"') {
            inQuotes = !inQuotes;
            if (inQuotes && tempArgument == NULL) tempArgument = (char*) malloc(1);
        }

        if (tempArgument != NULL) {
            tempArgument = realloc(tempArgument, strlen(tempArgument) + strlen(token));
            strcat(tempArgument, " ");
            strcat(tempArgument, token);
    
            if (inQuotes) 
                continue;
            else {
                // Eliminar caracteres de memoria del principio y las comillas.
                char *quotePosition = strpbrk(tempArgument, "\'\"");
                if (quotePosition != NULL) memmove(tempArgument, quotePosition, strlen(quotePosition) + 1);
                tempArgument += 1;
                tempArgument[strlen(tempArgument) - 1] = '\0';
                token = tempArgument;
                tempArgument = NULL;
            }
        }

        // Reasignar memoria para el nuevo argumento.
        *arguments = realloc(*arguments, (*argumentCount + 1) * sizeof(char*));

        // Agregar el nuevo.
        (*arguments)[*argumentCount] = strdup(token);
        (*argumentCount) += 1;
    }
    
    free(tempArgument);
    free(token);
}