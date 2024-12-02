#ifndef PROCESS_H
#define PROCESS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct process_t {
    char* name;
    int burstTime;
    int blockSize;
} process_t;

typedef struct processNode_t {
    process_t process_t;
    struct processNode_t *next;
} processNode_t;

void firstComeFirstServed(const char*);
void shortestJobFirst();
void roundRobin(char**, int);
void mkprocess(char**, int);
void lsprocesses();
void killprocess(char**, int);

#endif