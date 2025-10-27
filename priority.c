#include <stdio.h>
#include <stdlib.h>

struct Process {
    int name;
    int at;   // Arrival Time
    int bt;   // Burst Time
    int pr;   // Priority
    int ct;   // Completion Time
    int tat;  // Turnaround Time
    int wt;   // Waiting Time
} processes[100];

void sortProcesses(struct Process processes[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (processes[j].at > processes[j + 1].at) {
                struct Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }
}

int findHighestPriority(struct Process processes[], int n, int currt, int completed[]) {
    int highest_priority = 10000;
    int min_index = -1;

    for (int i = 0; i < n; i++) {
        if (!completed[i] && processes[i].at <= currt && processes[i].pr < highest_priority) {
            highest_priority = processes[i].pr;
            min_index = i;
        }
    }
    return min_index;
}

int main() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        processes[i].name = i + 1;
        printf("Enter arrival time, burst time, and priority for process P%d: ", i + 1);
        scanf("%d %d %d", &processes[i].at, &processes[i].bt, &processes[i].pr);
    }

    int currt = 0;
    sortProcesses(processes, n);

    int completed[n], compl = 0;
    for (int i = 0; i < n; i++) completed[i] = 0;

    // Arrays to keep execution order for Gantt chart
    int order[100], times[100], idx = 0;

    while (compl < n) {
        int i = findHighestPriority(processes, n, currt, completed);

        if (i == -1) {  // no process ready, CPU idle
            currt++;
            continue;
        }

        currt += processes[i].bt;
        processes[i].ct = currt;
        processes[i].tat = processes[i].ct - processes[i].at;
        processes[i].wt = processes[i].tat - processes[i].bt;

        completed[i] = 1;
        compl++;

        // Save order for Gantt chart
        order[idx] = processes[i].name;
        times[idx] = processes[i].ct;
        idx++;
    }

    
    printf("\nGantt chart : \n");
    for (int i = 0; i < idx; i++) {
        printf("|\tP%d\t", order[i]);
    }
    printf("|\n");

    printf("0\t");
    for (int i = 0; i < idx; i++) {
        printf("\t%d\t", times[i]);
    }
    printf("\n");
    

    float avgtat = 0.0, avgwt = 0.0;
    printf("\nProcess  ArrivalTime BurstTime Priority TurnaroundTime WaitingTime CompletionTime\n");
    for (int i = 0; i < n; i++) {
        printf("P%d%10d%10d%10d%15d%15d%15d\n", processes[i].name,
               processes[i].at, processes[i].bt, processes[i].pr,
               processes[i].tat, processes[i].wt, processes[i].ct);
        avgwt += processes[i].wt;
        avgtat += processes[i].tat;
    }

    printf("\nAverage Waiting Time = %.2f\n", avgwt / n);
    printf("Average Turnaround Time = %.2f\n", avgtat / n);

    return 0;
}
