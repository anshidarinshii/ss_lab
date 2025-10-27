#include <stdio.h>
#include <stdlib.h>

// Structure to store process details
struct Process {
    int name;        // process ID
    int arrival;     // arrival time
    int burst;       // burst time
    int remaining;   // remaining burst
    int status;      // 0 = not in queue, 1 = in queue or completed
    int completion;  // completion time
    int waiting;     // waiting time
    int turnaround;  // turnaround time
} processes[100];

// Queue node
struct Node {
    struct Process process;
    struct Node* next;
};

// Queue pointers
struct Node *front = NULL, *rear = NULL;

// Enqueue function
void enqueue(struct Process process) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->process = process;
    newNode->next = NULL;
    if (front == NULL)
        front = rear = newNo
