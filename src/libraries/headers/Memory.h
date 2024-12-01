#ifndef MEMORY_H    
#define MEMORY_H

#include "Process.h"

enum BlockStatus {
    FREE,
    OCCUPIED
} typedef BlockStatus;

typedef struct memoryBlock_t {
    process_t process_t;
    int address;
    BlockStatus status;
    struct memoryBlock_t* next;
    // int blockSize; blockSize se encuentra en process_t
} memoryBlock_t;

// Lista de procesos
extern processNode_t* processList;

// Lista de bloques de memoria
extern memoryBlock_t* memoryList;

// Tamaño de la memoria
extern int memorySize;

void allocate(char**, int);
void bestFit(char*);
void worstFit(char*);
void firstFit(char*);
void testFit(char*);
void freeprocess(char**, int);
void memorystatus();
void compactmemory();

#endif