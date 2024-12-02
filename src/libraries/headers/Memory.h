#ifndef MEMORY_H    
#define MEMORY_H

#include "Process.h"

enum BlockStatus {
    FREE,
    OCCUPIED
} typedef BlockStatus;

typedef struct memoryBlock_t {
    char *processName;
    int address;
    BlockStatus status;
    struct memoryBlock_t* next;
    int blockSize;
} memoryBlock_t;

// Lista de procesos
extern processNode_t* processList;

// Lista de bloques de memoria
extern memoryBlock_t* memoryList;

// Tamaño de la memoria
extern int memorySize;

void allocate(char**, int);
void bestFit(process_t);
void worstFit(process_t process);
void firstFit(process_t process);
void freeprocess(char**, int);
void memorystatus();
void compactmemory();

#endif