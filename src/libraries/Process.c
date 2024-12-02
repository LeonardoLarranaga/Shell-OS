#include "Process.h"
#include "Memory.h"

// Función para intercambiar dos procesos
void swap(processNode_t *a, processNode_t *b) {  
    process_t temp = a->process_t;  
    a->process_t = b->process_t;  
    b->process_t = temp;  
}

// Función para ordenar los procesos por burst time
void bubbleSort(processNode_t *start) {  
    int swapped, i;  
    processNode_t *ptr1;  
    processNode_t *lptr = NULL;  
  
    if (start == NULL) return;  
  
    do {  
        swapped = 0;  
        ptr1 = start;  
  
        while (ptr1->next != lptr) {  
            if (ptr1->process_t.burstTime > ptr1->next->process_t.burstTime) {  
                swap(ptr1, ptr1->next);  
                swapped = 1;  
            }  
            ptr1 = ptr1->next;  
        }  
        lptr = ptr1;  
    }  while (swapped);  
}  
  
// Función para crear un proceso
// uso: mkprocess <processId> <burstTime> <blockSize>
void mkprocess(char** arguments, int argumentCount) {
    if (argumentCount != 3) {
        printf("mkprocess: Expected three arguments.\n");
        return;
    }

    // Verificar que el burst time y block size sea mayor a 0
    char* name = arguments[0];
    int burstTime = atoi(arguments[1]);
    if (burstTime <= 0) {
        printf("mkprocess: Burst time must be greater than 0.\n");
        return;
    }

    int blockSize = atoi(arguments[2]);
    if (blockSize <= 0) {
        printf("mkprocess: Block size must be greater than 0.\n");
        return;
    }
    
    // Verificar que el proceso no exista
    processNode_t* current = processList;
    while (current != NULL) {
        if (!strcmp(current->process_t.name, name)) {
            printf("mkprocess: Process '%s' already exists.\n", name);
            return;
        }
        current = current->next;
    }

    // Crear un nuevo proceso
    processNode_t* newNode = (processNode_t*) malloc(sizeof(processNode_t));
    // Asignar propiedades al nuevo proceso
    newNode->process_t.name = (char*)malloc(strlen(name) + 1);
    strcpy(newNode->process_t.name, name);
    newNode->process_t.burstTime = burstTime;
    newNode->process_t.blockSize = blockSize;
    newNode->next = NULL;

    // Si la lista de procesos está vacía, asignar el nuevo proceso como el primero
    if (processList == NULL) {
        processList = newNode;
        return;
    }

    // Agregar el nuevo proceso al final de la lista
    current = processList;
    while (current->next != NULL) current = current->next;
    current->next = newNode;
}

// Función para imprimir los procesos
void lsprocesses() {
    processNode_t* current = processList;
    if (current == NULL) {
        printf("lsprocesses: No processes in ready queue.\n");
        return;
    }

    while (current != NULL) {
        printf("Process '%s': %dt | %dbs \n", current->process_t.name, current->process_t.burstTime, current->process_t.blockSize);
        current = current->next;
    }
}

// Función para liberar un proceso
void freeProcessNode(processNode_t* process) {
    if (process == NULL) return;
    if (process->process_t.name != NULL) {
        free(process->process_t.name);
        process->process_t.name = NULL;
    }
    free(process);
}

// Función para ejecutar el algoritmo de scheduling First Come First Served
// TODO: cuando se hace kill al final de los procesos, se queda un cuadro extra arriba
void firstComeFirstServed(const char* algorithm) {
    if (processList == NULL) {
        printf("FCFS scheduling: No processes in ready queue.\n");
        return;
    }

    // Verificar que haya procesos cargados en memoria
    memoryBlock_t* current = memoryList;
    while (current != NULL) {
        if (current->status == OCCUPIED) break;
        current = current->next;
    }

    if (current == NULL || current->status == FREE) {
        printf("FCFS scheduling: No processes in memory.\n");
        return;
    }

    printf("%s scheduling:\n", algorithm);
    printf("%-8s %-15s %-12s %-14s %-18s\n", 
        "Time", 
        "Process", 
        "Burst Time", 
        "Waiting Time", 
        "Turnaround Time"
    );

    int time = 0, totalWaitingTime = 0, totalTurnaroundTime = 0, waitingTime = 0, turnaroundTime = 0, burstTime = 0, processCount = 0;

    current = memoryList;
    processNode_t* processInBlock = (processNode_t*) malloc(sizeof(processNode_t));
    while (current != NULL) {
        if (current->status != OCCUPIED) {
            current = current->next;
            continue;
        }

        findProcess(current->processName, &processInBlock);        
        if (processInBlock == NULL) {
            current = current->next;
            continue;
        }

        burstTime = processInBlock->process_t.burstTime;
        waitingTime = time;
        turnaroundTime = waitingTime + burstTime;

        printf("%-8d %-15s %-12d %-14d %-18d\n",
            time,
            processInBlock->process_t.name,
            burstTime,
            waitingTime,
            turnaroundTime
        );

        time += burstTime;
        totalWaitingTime += waitingTime;
        totalTurnaroundTime += turnaroundTime;

        current = current->next;
        processCount += 1;
    }


    printf("------------------------------------------------------------\n");
    printf("%-8s %-15s %-12s %-14.2f %-18.2f\n", 
        "Average", "", "",
       (float) totalWaitingTime / processCount,
       (float) totalTurnaroundTime / processCount
    );

    // Kill procesos cargados en memoria
    current = memoryList;
    while (current != NULL) {
        if (current->status == OCCUPIED) {
            killprocess(&current->processName, 1);
        }
        current = current->next;
    }

}

// Función para ejecutar el algoritmo de scheduling Shortest Job First
void shortestJobFirst() {
    // Ordenar los procesos por burst time
    bubbleSort(processList);
    // Ejecutar el algoritmo de scheduling FCFS
    firstComeFirstServed("SJF");
}

// Función para ejecutar el algoritmo de scheduling Round Robin
// TODO: Cambiar processList por el bloque de memoria con los procesos cargados. Agregar freeProcess al final del while para liberar memoria
void roundRobin(char** arguments, int argumentCount) {
    // Si no se especifica un quantum, se usa el valor por defecto de 10
    int timeQuantum = 10;
    if (argumentCount == 1) {
        timeQuantum = atoi(arguments[0]);
        if (timeQuantum <= 0) {
            printf("roundrobin: Quantum must be greater than 0.\n");
            return;
        }
    } else if (argumentCount != 0) {
        printf("roundrobin: Expected only one optional argument.\n");
        return;
    }

    if (processList == NULL) {
        printf("Round Robin scheduling: No processes in ready queue.\n");
        return;
    }

    int time = 0, totalWaitingTime = 0, totalTurnaroundTime = 0, processCount = 0;

    // Contar la cantidad de procesos
    for (processNode_t* temp = processList; temp != NULL; temp = temp->next) processCount += 1;

    // Crear un arreglo con los burst times restantes de los procesos
    int remainingBurstTimes[processCount];
    int index = 0;
    for (processNode_t* temp = processList; temp != NULL; temp = temp->next) {
        remainingBurstTimes[index] = temp->process_t.burstTime;
        index += 1;
    }
        
    // Crear arreglos para los waiting times y turnaround times
    int waitingTimes[processCount], turnaroundTimes[processCount];
    for (int i = 0; i < processCount; i++) {
        waitingTimes[i] = 0;
        turnaroundTimes[i] = 0;
    }

    printf("Round Robin scheduling. Q = %dt:\n", timeQuantum);
    processNode_t* current = processList;

    int finishedProcesses = 0;
    while (finishedProcesses < processCount) {
        index = 0;
        current = processList;

        while (current != NULL) {
            if (remainingBurstTimes[index] > 0) {
                // Ejecutar el proceso actual por el tiempo quantum o el burst time restante
                int executionTime = remainingBurstTimes[index] > timeQuantum ? timeQuantum : remainingBurstTimes[index];

                // Imprimir el proceso que se está ejecutando
                printf("Executing process '%s' at %dt for %dt ", 
                    current->process_t.name, 
                    time, 
                    executionTime
                );

                time += executionTime;
                remainingBurstTimes[index] -= executionTime;
                printf("(remaining %dt)\n", remainingBurstTimes[index]);

                // Actualizar los waiting times y turnaround times
                if (remainingBurstTimes[index] == 0) {
                    turnaroundTimes[index] = time;
                    waitingTimes[index] = turnaroundTimes[index] - current->process_t.burstTime;
                    totalTurnaroundTime += turnaroundTimes[index];
                    totalWaitingTime += waitingTimes[index];
                    finishedProcesses += 1;
                }
            }

            current = current->next;
            index += 1;
        }
    }

    // Imprimir los waiting times y turnaround times
    printf("\n%-15s %-12s %-14s %-18s\n", 
        "Process", 
        "Burst Time", 
        "Waiting Time", 
        "Turnaround Time"
    );

    index = 0;
    current = processList;
    while (current != NULL) {
        printf("%-15s %-12d %-14d %-18d\n",
            current->process_t.name,
            current->process_t.burstTime,
            waitingTimes[index],
            turnaroundTimes[index]
        );

        current = current->next;
        index += 1;
    }

    printf("------------------------------------------------------------\n");
    printf("%-15s %-12s %-14.2f %-18.2f\n", 
        "Average", "",
       (float) totalWaitingTime / processCount,
       (float) totalTurnaroundTime / processCount
    );

    // Liberar memoria asignada
    for (int i = 0; i < processCount; i++) {
        freeProcessNode(processList);
        processList = processList->next;
    }
}

// Función para matar un proceso (quitar de la ready queue)
// uso: killprocess <processId> <process2Id> ...
void killprocess(char** arguments, int argumentCount) {
    if (argumentCount <= 0) {
        printf("killprocess: Expected at least one argument.\n");
        return;
    }

    for (int i = 0; i < argumentCount; i++) {
        char* processId = arguments[i];
        processNode_t* current = processList;
        processNode_t* previous = NULL;

        while (current != NULL) {
            if (!strcmp(current->process_t.name, processId)) {
                if (previous == NULL) {
                    processList = current->next;
                } else {
                    previous->next = current->next;
                }

                freeprocess(&current->process_t.name, 1);
                break;
            }

            previous = current;
            current = current->next;
        }

        if (current == NULL) printf("killprocess: Process '%s' not found.\n", processId);
    }
}