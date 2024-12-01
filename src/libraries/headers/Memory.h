#ifndef MEMORY_H    
#define MEMORY_H

#include "Process.h"

// Lista de procesos
extern processNode_t* processList;

// TODO: Implementar la estructura de datos para la memoria
typedef struct memoryBlock_t {
    int size;
    int processId;
    struct memoryBlock_t* next;
} memoryBlock_t;

void allocate(char**);
void bestFit(char*);
void worstFit(char*);
void firstFit(char*);

#endif