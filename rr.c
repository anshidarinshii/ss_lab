#include <stdio.h>
#include <stdlib.h>

struct Process {
    int name;
    int arrival;
    int burst;
    int remaining;
    int status;
    int completion;
    int waiting;
    int turnaround;
} processes[100];

struct Node {
    struct Process process;
    struct Node* next;
};

struct Node *front = NULL;
struct Node *rear = NULL;

void enqueue(struct Process process) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->process = process;
    newNode->next = NULL;

    if (front == NULL) {
        front = rear = newNode;
    } else {
        rear->next = newNode;
        rear = newNode;
    }
}

struct Process dequeue() {
    struct Node* temp = front;
    struct Process process = temp->process;
    front = front->next;
    free(temp);
    return process;
}

void printqueue() {
    struct Node *temp = front;
    while (temp != NULL) {
        printf("%d-->", temp->process.name);
        temp = temp->next;
    }
    printf("NULL\n");
}

int main() {
    int n, quantum;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        processes[i].name = i + 1;
        printf("Enter arrival time and burst time for process P%d: ", i + 1);
        scanf("%d %d", &processes[i].arrival, &processes[i].burst);
        processes[i].remaining = processes[i].burst;
        processes[i].status = 0;
        processes[i].completion = -1;
        processes[i].waiting = -1;
        processes[i].turnaround = -1;
    }

    printf("Enter the time quantum: ");
    scanf("%d", &quantum);

    int currenttime = 0;
    int completed = 0;

    
    int pr[100];   
    int prI[100];  
    int pi = 0;    

    enqueue(processes[0]);
    processes[0].status = 1;

    while (completed < n) {
        if (front != NULL) {
            struct Process currentProcess = dequeue();
            int execution_time = (currentProcess.remaining < quantum) ? currentProcess.remaining : quantum;
            currentProcess.remaining -= execution_time;
            currenttime += execution_time;

            for (int i = 0; i < n; i++) {
                if (processes[i].arrival <= currenttime && processes[i].status != 1 && processes[i].remaining > 0) {
                    enqueue(processes[i]);
                    processes[i].status = 1;
                }
            }

        
            pr[pi] = currentProcess.name;
            prI[pi] = currenttime;
            pi++;

            if (currentProcess.remaining <= 0) {
                completed++;
                for (int i = 0; i < n; i++) {
                    if (processes[i].name == currentProcess.name) {
                        processes[i].completion = currenttime;
                        break;
                    }
                }
            } else {
                currentProcess.status = 1;
                enqueue(currentProcess);
            }
        } else {
            currenttime++;
        }
    }

    
    printf("\nGantt chart : \n");
    for (int i = 0; i < pi; i++) {
        printf("|\tP%d", pr[i]);
    }
    printf("|\n");

    printf("0 ");
    for (int i = 0; i < pi; i++) {
        printf("\t%d", prI[i]);
    }
    printf("\n");
    

    float avwt = 0.0;
    float avtat = 0.0;
    printf("\nProcess ArrivalTime BurstTime  CompletionTime  TurnAroundTime WaitingTime\n");
    for (int i = 0; i < n; i++) {
        processes[i].turnaround = processes[i].completion - processes[i].arrival;
        processes[i].waiting = processes[i].turnaround - processes[i].burst;
        printf("P%d%10d%10d%15d%15d%15d\n",
               processes[i].name, processes[i].arrival, processes[i].burst,
               processes[i].completion, processes[i].turnaround, processes[i].waiting);
        avwt += processes[i].waiting;
        avtat += processes[i].turnaround;
    }
    printf("\nAverage Waiting Time = %f\n", avwt / n);
    printf("Average Turn Around Time = %f\n", avtat / n);

    return 0;
}
