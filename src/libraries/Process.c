#include "Process.h"

void swap(processNode_t *a, processNode_t *b) {  
    process_t temp = a->process_t;  
    a->process_t = b->process_t;  
    b->process_t = temp;  
}

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
  
processNode_t* processList = NULL;

void internalMKProcess(char** arguments) {
    char* name = arguments[0];
    int burstTime = atoi(arguments[1]);
    if (burstTime <= 0) {
        printf("mkprocess: Burst time must be greater than 0.\n");
        return;
    }

    processNode_t* newNode = (processNode_t*) malloc(sizeof(processNode_t));
    newNode->process_t.name = (char*)malloc(strlen(name) + 1);
    strcpy(newNode->process_t.name, name);
    newNode->process_t.burstTime = burstTime;
    newNode->next = NULL;

    if (processList == NULL) {
        processList = newNode;
        return;
    }

    processNode_t* current = processList;
    while (current->next != NULL) current = current->next;
    current->next = newNode;
}

void printProcesses() {
    processNode_t* current = processList;
    if (current == NULL) {
        printf("lsprocesses: No processes in ready queue.\n");
        return;
    }

    while (current != NULL) {
        printf("Process '%s': %dt\n", current->process_t.name, current->process_t.burstTime);
        current = current->next;
    }
}

void freeProcess(processNode_t* process) {
    free(process->process_t.name);
    free(process);
}

void firstComeFirstServed(const char* algorithm) {
    if (processList == NULL) {
        printf("FCFS scheduling: No processes in ready queue.\n");
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

    while (processList != NULL) {
        burstTime = processList->process_t.burstTime;
        waitingTime = time;
        turnaroundTime = waitingTime + burstTime;

        printf("%-8d %-15s %-12d %-14d %-18d\n",
            time,
            processList->process_t.name,
            burstTime,
            waitingTime,
            turnaroundTime
        );

        time += burstTime;
        totalWaitingTime += waitingTime;
        totalTurnaroundTime += turnaroundTime;

        freeProcess(processList);
        processList = processList->next;
        processCount += 1;
    }

    printf("------------------------------------------------------------\n");
    printf("%-8s %-15s %-12s %-14.2f %-18.2f\n", 
        "Average", "", "",
       (float) totalWaitingTime / processCount,
       (float) totalTurnaroundTime / processCount
    );
}

void shortestJobFirst() {
    bubbleSort(processList);
    firstComeFirstServed("SJF");
}

void internalRoundRobin(int timeQuantum) {
    if (processList == NULL) {
        printf("Round Robin scheduling: No processes in ready queue.\n");
        return;
    }

    int time = 0, totalWaitingTime = 0, totalTurnaroundTime = 0, processCount = 0;

    for (processNode_t* temp = processList; temp != NULL; temp = temp->next) processCount += 1;

    int remainingBurstTimes[processCount];
    int index = 0;
    for (processNode_t* temp = processList; temp != NULL; temp = temp->next) {
        remainingBurstTimes[index] = temp->process_t.burstTime;
        index += 1;
    }
        
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
                int executionTime = remainingBurstTimes[index] > timeQuantum ? timeQuantum : remainingBurstTimes[index];

                printf("Executing process '%s' at %dt for %dt ", 
                    current->process_t.name, 
                    time, 
                    executionTime
                );

                time += executionTime;
                remainingBurstTimes[index] -= executionTime;
                printf("(remaining %dt)\n", remainingBurstTimes[index]);

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

    for (int i = 0; i < processCount; i++) {
        freeProcess(processList);
        processList = processList->next;
    }
}