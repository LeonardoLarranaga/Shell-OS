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

    // Verificar que el proceso no esté cargado en memoria y este en la lista de procesos
    memoryBlock_t* current = memoryList;
    processNode_t* currentProcesses = processList;

    while (current != NULL) {
        if(current->status == OCCUPIED){
            if (!strcmp(current->processName, processId)) {
                printf("allocate: Process '%s' is already in memory.\n", processId);
                return;
            }
        }
        current = current->next;
    }

    while (currentProcesses != NULL) {
        if (!strcmp(currentProcesses->process_t.name, processId)) {

            if (!strcmp(strategy, "bestfit")) {
                bestFit(currentProcesses->process_t);
            } else if (!strcmp(strategy, "worstfit")) {
                worstFit(currentProcesses->process_t);
            } else if (!strcmp(strategy, "firstfit")) {
                firstFit(currentProcesses->process_t);
            } else {
                printf("allocate: Invalid strategy.\n");
            }
            return;
        } 
        currentProcesses = currentProcesses->next;
    }
    printf("allocate: Process '%s' not found.\n", processId);
}

// Función para asignar memoria con el algoritmo Best Fit
void firstFit(process_t process) {
    memoryBlock_t* current = memoryList;

    while (current != NULL) {
        if (current->status == FREE && current->blockSize >= process.blockSize) {
            if (current->blockSize > process.blockSize) {
                memoryBlock_t* newFreeBlock = malloc(sizeof(memoryBlock_t));
                if (newFreeBlock == NULL) {
                    printf("Error: No se pudo asignar memoria para el nuevo bloque.\n");
                    return;
                }

                newFreeBlock->processName = "";
                newFreeBlock->address = current->address + process.blockSize;
                newFreeBlock->status = FREE;
                newFreeBlock->blockSize = current->blockSize - process.blockSize;
                newFreeBlock->next = current->next;

                current->next = newFreeBlock;
            }

            current->processName = process.name;
            current->status = OCCUPIED;
            current->blockSize = process.blockSize;

            return;
        }

        current = current->next;
    }

    printf("No se encontró un bloque adecuado para el proceso '%s'.\n", process.name);
}


// Función para asignar memoria con el algoritmo Worst Fit
void worstFit(process_t process) {
    // TODO: Implementar la asignación de memoria con el algoritmo Worst Fit
    printf("allocate: Worst fit strategy not implemented.\n");
}

// Función para asignar memoria con el algoritmo First Fit
void bestFit(process_t process) {
    memoryBlock_t* current = memoryList;
    memoryBlock_t* best = NULL;

    // Buscar el mejor bloque libre
    while (current != NULL) {
        if (current->status == FREE && current->blockSize >= process.blockSize) {
            if (best == NULL || current->blockSize < best->blockSize) {
                best = current;
            }
        }
        current = current->next;
    }

    // Si no se encontró un bloque adecuado
    if (best == NULL) {
        printf("Error: No hay suficiente memoria para el proceso '%s'.\n", process.name);
        return;
    }

    // Dividir el bloque si sobra espacio
    if (best->blockSize > process.blockSize) {
        memoryBlock_t* newFreeBlock = malloc(sizeof(memoryBlock_t));
        if (newFreeBlock == NULL) {
            printf("Error: No se pudo asignar memoria para el nuevo bloque.\n");
            return;
        }

        newFreeBlock->processName = "";
        newFreeBlock->address = best->address + process.blockSize;
        newFreeBlock->status = FREE;
        newFreeBlock->blockSize = best->blockSize - process.blockSize;
        newFreeBlock->next = best->next;

        best->next = newFreeBlock;
    }

    // Asignar el bloque al proceso
    best->processName = process.name;
    best->status = OCCUPIED;
    best->blockSize = process.blockSize;
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

        while (current != NULL) {
            if (!strcmp(current->processName, processId)) {
                if (current->next != NULL && current->next->status == FREE) {
                    memoryBlock_t* freeBlock = current->next; 
                    current->blockSize += freeBlock->blockSize; 
                    current->next = freeBlock->next; 
                    free(freeBlock); 
                }

                current->processName = "";
                current->status = FREE;
                return;
            }

            current = current->next;
        }

        if (current == NULL) {
            printf("freeprocess: Process '%s' not found.\n", processId);
        }
    }
}


// Función para ver el estado de la memoria
void memorystatus() {
    memoryBlock_t* current = memoryList;
    
    if (current == NULL) {
        printf("memoryStatus: No processes in memory.\n");
        return;
    }

    while (current != NULL) {
        int totalWidth = 25; // Ancho total para el marco
        char blockStatusStr[10]; // Espacio suficiente para "OCCUPIED"
        strcpy(blockStatusStr, current->status == OCCUPIED ? "OCCUPIED" : "FREE");

        // Calcular relleno para centrar el nombre
        char *name = "Memory free";
        int nameLength = strlen(name);
        int namePadding = (totalWidth - nameLength) / 2;
        int nameExtraPadding = (totalWidth - nameLength) % 2;
        
        
        if(strcmp(blockStatusStr, "OCCUPIED") == 0){
            name = current->processName;
            nameLength = strlen(name);
            namePadding = (totalWidth - nameLength) / 2;
            nameExtraPadding = (totalWidth - nameLength) % 2;
        }

        // Calcular relleno para centrar el adress del bloque
        char blockAddressStr[12]; // Para convertir blockSize a string
        sprintf(blockAddressStr, "Address: %d", current->address);
        int blockAddressLength = strlen(blockAddressStr);
        int sizeAddressPadding = (totalWidth - blockAddressLength) / 2;
        int sizeAdressExtraPadding = (totalWidth - blockAddressLength) % 2;    
        
        // Calcular relleno para centrar el tamaño del bloque
        char blockSizeStr[12]; // Para convertir blockSize a string
        sprintf(blockSizeStr, "Size: %d", current->blockSize);
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
        printf("%s", name);
        for (int i = 0; i < namePadding + nameExtraPadding; i++) printf(" ");
        printf("|\n");
        
        // Imprimir direccion
        printf("|");
        for (int i = 0; i < sizeAddressPadding; i++) printf(" ");
        printf("%s", blockAddressStr);
        for (int i = 0; i < sizeAddressPadding + sizeAdressExtraPadding; i++) printf(" ");
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
void compactmemory() {
    memoryBlock_t* current = memoryList;
    memoryBlock_t* previous = NULL;
    int memoryAcum = 0;

    while (current != NULL) {
        current->address = current->address - memoryAcum;

        if (current->status == FREE && current->next != NULL) {
            if(current->next->status == OCCUPIED){
                if (previous == NULL) {
                    memoryList = current->next; 
                    memoryAcum += current->blockSize;
                    current = current->next;
                } else {
                    previous->next = current->next;                     
                    memoryAcum += current->blockSize;
                    free(current);                  
                    current = previous->next;      
                }
            }
        } else {
            previous = current;
            current = current->next;
        }

        if(current == NULL){
            if(previous->status == OCCUPIED){
                memoryBlock_t* newFreeBlock = malloc(sizeof(memoryBlock_t));
                if (newFreeBlock == NULL) {
                    printf("Error: No se pudo asignar memoria para el nuevo bloque.\n");
                    return;
                }
                newFreeBlock->processName = "";
                newFreeBlock->address =  previous->address + previous->blockSize;
                newFreeBlock->status = FREE;
                newFreeBlock->blockSize = memoryAcum;
                newFreeBlock->next = NULL;
                previous->next = newFreeBlock;
            } else {
                previous->blockSize += memoryAcum;
            }
        }
    }
}
