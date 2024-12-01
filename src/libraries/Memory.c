#include "Memory.h"

void allocate(char** arguments) {
    char* processId = arguments[0];
    char* strategy = arguments[1];

    if (!strcmp(strategy, "bestfit")) {
        bestFit(processId);
    } else if (!strcmp(strategy, "worstfit")) {
        worstFit(processId);
    } else if (!strcmp(strategy, "firstfit")) {
        firstFit(processId);
    } else {
        printf("allocate: Invalid strategy.\n");
    }
}

void bestFit(char* processId) {
    // TODO: Implementar la asignación de memoria con el algoritmo Best Fit
    printf("allocate: Best fit strategy not implemented.\n");
}

void worstFit(char* processId) {
    // TODO: Implementar la asignación de memoria con el algoritmo Worst Fit
    printf("allocate: Worst fit strategy not implemented.\n");
}

void firstFit(char* processId) {
    // TODO: Implementar la asignación de memoria con el algoritmo First Fit
    printf("allocate: First fit strategy not implemented.\n");
}