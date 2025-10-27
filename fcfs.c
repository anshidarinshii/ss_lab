/*
 * C Program for FCFS (First-Come, First-Serve) Scheduling
 *
 * This program calculates CT, TAT, and WT for a set of processes
 * and displays the Gantt Chart.
 */

#include <stdio.h>

// Define a structure for a process
struct process {
    int pid; // Process ID
    int at;  // Arrival Time
    int bt;  // Burst Time
    int ct;  // Completion Time
    int tat; // Turn-Around Time
    int wt;  // Waiting Time
};

// Structure to store Gantt chart blocks
struct GanttBlock {
    int pid;
    int startTime;
    int endTime;
};

// Function to swap two processes (used in sorting)
void swap(struct process *xp, struct process *yp) {
    struct process temp = *xp;
    *xp = *yp;
    *yp = temp;
}

// Function to sort processes based on Arrival Time (Bubble Sort)
void sortProcesses(struct process proc[], int n) {
    int i, j;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (proc[j].at > proc[j + 1].at) {
                swap(&proc[j], &proc[j + 1]);
            }
        }
    }
}

// Function to print the Gantt Chart
void printGanttChart(struct GanttBlock gantt[], int n) {
    int i;
    printf("\n\n--- Gantt Chart ---\n\n");

    // Print top border
    for (i = 0; i < n; i++) {
        printf("---------");
    }
    printf("-\n");

    // Print Process IDs
    for (i = 0; i < n; i++) {
        if (gantt[i].pid == 0) {
            printf("|  IDLE  ");
        } else {
            printf("|   P%d   ", gantt[i].pid);
        }
    }
    printf("|\n");

    // Print bottom border
    for (i = 0; i < n; i++) {
        printf("---------");
    }
    printf("-\n");

    // Print times
    printf("%-9d", gantt[0].startTime);
    for (i = 0; i < n; i++) {
        printf("%-9d", gantt[i].endTime);
    }
    printf("\n");
}

// Function to find Completion Time, TAT, and WT
void findTimes(struct process proc[], int n) {
    int i;
    int current_time = 0;
    float total_wt = 0;
    float total_tat = 0;
    
    struct GanttBlock gantt[50]; // Array for Gantt chart
    int ganttIndex = 0;

    // Sort processes by arrival time
    sortProcesses(proc, n);

    printf("\n--- FCFS Execution Order ---\n");
    for (i = 0; i < n; i++) {
        // If the current process arrives after the previous one finished,
        // the CPU is idle.
        if (current_time < proc[i].at) {
            printf("... CPU Idle from %d to %d ...\n", current_time, proc[i].at);
            gantt[ganttIndex].pid = 0; // 0 for IDLE
            gantt[ganttIndex].startTime = current_time;
            gantt[ganttIndex].endTime = proc[i].at;
            ganttIndex++;
            
            current_time = proc[i].at;
        }
        
        printf("Process P%d starts at time %d\n", proc[i].pid, current_time);

        // Add process to Gantt chart
        gantt[ganttIndex].pid = proc[i].pid;
        gantt[ganttIndex].startTime = current_time;

        // Calculate Completion Time
        proc[i].ct = current_time + proc[i].bt;
        
        // Update current time
        current_time = proc[i].ct;

        // Finish Gantt block
        gantt[ganttIndex].endTime = current_time;
        ganttIndex++;

        // Calculate Turn-Around Time
        proc[i].tat = proc[i].ct - proc[i].at;

        // Calculate Waiting Time
        proc[i].wt = proc[i].tat - proc[i].bt;

        // Add to totals
        total_wt += proc[i].wt;
        total_tat += proc[i].tat;
    }

    // Print the results table
    printf("\n--- FCFS Scheduling Results ---\n");
    printf("PID\tAT\tBT\tCT\tTAT\tWT\n");
    for (i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n",
               proc[i].pid, proc[i].at, proc[i].bt,
               proc[i].ct, proc[i].tat, proc[i].wt);
    }

    // Print averages
    printf("\nAverage Waiting Time: %.2f\n", total_wt / n);
    printf("Average Turn-Around Time: %.2f\n", total_tat / n);
    
    // Print Gantt Chart
    printGanttChart(gantt, ganttIndex);
}

int main() {
    int n, i;
    struct process proc[20]; // Array of processes

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    printf("Enter Arrival Time and Burst Time for each process:\n");
    for (i = 0; i < n; i++) {
        printf("P%d: ", i + 1);
        proc[i].pid = i + 1; // Set PID
        scanf("%d %d", &proc[i].at, &proc[i].bt);
    }

    findTimes(proc, n);

    return 0;
}
