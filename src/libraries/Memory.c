#include "Memory.h"

// Función para asignar memoria a un proceso
// uso: allocate <processId> <strategy>
void allocate(char** arguments, int argumentCount) {
    if (argumentCount != 2) {
        printf("allocate: Expected two arguments.\n");
        return;
    }

    char* processId = arguments[0];
    char* strategy = arguments[1];

    // Verificar que el proceso no esté cargado en memoria
    memoryBlock_t* current = memoryList;
    while (current != NULL) {
        if (!strcmp(current->process_t.name, processId)) {
            printf("allocate: Process '%s' is already in memory.\n", processId);
            return;
        }
        current = current->next;
    }

    if (!strcmp(strategy, "bestfit")) {
        bestFit(processId);
    } else if (!strcmp(strategy, "worstfit")) {
        worstFit(processId);
    } else if (!strcmp(strategy, "firstfit")) {
        firstFit(processId);
    } else if (!strcmp(strategy, "testfit")) {
        testFit(processId);
    } else {
        printf("allocate: Invalid strategy.\n");
    }
}

// Función para asignar memoria con el algoritmo Best Fit
void bestFit(char* processId) {
    // TODO: Implementar la asignación de memoria con el algoritmo Best Fit
    printf("allocate: Best fit strategy not implemented.\n");
}

// Función para asignar memoria con el algoritmo Worst Fit
void worstFit(char* processId) {
    // TODO: Implementar la asignación de memoria con el algoritmo Worst Fit
    printf("allocate: Worst fit strategy not implemented.\n");
}

// Función para asignar memoria con el algoritmo First Fit
void firstFit(char* processId) {
    // TODO: Implementar la asignación de memoria con el algoritmo First Fit
    printf("allocate: First fit strategy not implemented.\n");
}

// Función de prueba para asignar memoria
void testFit(char* processId) {
    processNode_t* current = processList;
    while (current != NULL) {
        if (!strcmp(current->process_t.name, processId)) {
            memoryBlock_t* newNode = (memoryBlock_t*) malloc(sizeof(memoryBlock_t));
            newNode->process_t = current->process_t;
            newNode->address = 0;
            newNode->status = OCCUPIED;
            newNode->next = memoryList; // Agregar al inicio de la lista
            memoryList = newNode;
            return;
        }
        current = current->next;
    }

    printf("allocate: Process '%s' not found.\n", processId);
}

// Función para liberar memoria de un proceso
// uso: freeprocess <processId> <process2Id> ...
void freeprocess(char** arguments, int argumentCount) {
    if (argumentCount <= 0) {
        printf("freeprocess: Expected at least one argument.\n");
        return;
    }

    for (int i = 0; i < argumentCount; i++) {
        char* processId = arguments[i];
        memoryBlock_t* current = memoryList;
        memoryBlock_t* previous = NULL;

        while (current != NULL) {
            if (!strcmp(current->process_t.name, processId)) {
                if (previous == NULL) {
                    memoryList = current->next;
                } else {
                    previous->next = current->next;
                }

                // TODO: Implementar la liberación de memoria
                printf("freeprocess: Memory deallocation not implemented.\n");
                // El proceso también debe ser eliminado de la lista de procesos
                // TODO: no borrarlo sino ponerlo como FREE ???????
                killprocess(&processId, 1);
                break;
            }

            previous = current;
            current = current->next;
        }

        if (current == NULL) printf("freeprocess: Process '%s' not found.\n", processId);   
    }
}

// Función para ver el estado de la memoria
void memorystatus() {
    memoryBlock_t* current = memoryList;
    if (current == NULL) {
        printf("memoryStatus: No processes in memory.\n");
        return;
    }

    // TODO: Implementar la impresión de los bloques de memoria (casi implementada). Se tienen que ver visualmente los bloques de memoria y los fragmentos libres (ya se ven los bloques de memoria, cuando haya fragmentos hay que probar que se vean)

    while (current != NULL) {
        int totalWidth = 25; // Ancho total para el marco
        char blockStatusStr[10]; // Espacio suficiente para "OCCUPIED"
        strcpy(blockStatusStr, current->status == OCCUPIED ? "OCCUPIED" : "FREE");

        // Calcular relleno para centrar el nombre
        int nameLength = strlen(current->process_t.name);
        int namePadding = (totalWidth - nameLength) / 2;
        int nameExtraPadding = (totalWidth - nameLength) % 2;

        // Calcular relleno para centrar el tamaño del bloque
        char blockSizeStr[12]; // Para convertir blockSize a string
        sprintf(blockSizeStr, "%d", current->process_t.blockSize);
        int blockSizeLength = strlen(blockSizeStr);
        int sizePadding = (totalWidth - blockSizeLength) / 2;
        int sizeExtraPadding = (totalWidth - blockSizeLength) % 2;

        // Calcular relleno para centrar el estado
        int statusLength = strlen(blockStatusStr);
        int statusPadding = (totalWidth - statusLength) / 2;
        int statusExtraPadding = (totalWidth - statusLength) % 2;

        // Imprimir el marco
        printf("---------------------------\n");

        // Imprimir el nombre del proceso
        printf("|");
        for (int i = 0; i < namePadding; i++) printf(" ");
        printf("%s", current->process_t.name);
        for (int i = 0; i < namePadding + nameExtraPadding; i++) printf(" ");
        printf("|\n");

        // Imprimir el tamaño del bloque
        printf("|");
        for (int i = 0; i < sizePadding; i++) printf(" ");
        printf("%s", blockSizeStr);
        for (int i = 0; i < sizePadding + sizeExtraPadding; i++) printf(" ");
        printf("|\n");

        // Imprimir el estado del bloque
        printf("|");
        for (int i = 0; i < statusPadding; i++) printf(" ");
        printf("%s", blockStatusStr);
        for (int i = 0; i < statusPadding + statusExtraPadding; i++) printf(" ");
        printf("|\n");

        current = current->next;
    }

    printf("---------------------------\n");
}

// Función para compactar la memoria
// TODO: Implementar la compactación de memoria
void compactmemory() {
    printf("compactmemory: Memory compaction not implemented.\n");
}