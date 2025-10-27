/*
 * C Program for SJF (Shortest Job First) Scheduling - Non-Preemptive
 *
 * Calculates CT, TAT, WT, and displays Gantt Chart.
 * Handles different arrival times.
 */

#include <stdio.h>

// Define a structure for a process
struct process {
    int pid;
    int at;
    int bt;
    int ct;
    int tat;
    int wt;
    int completed;
};

// Structure to store Gantt chart blocks
struct GanttBlock {
    int pid;
    int startTime;
    int endTime;
};

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
    int i, shortest_job_index;
    int current_time = 0;
    int completed_processes = 0;
    float total_wt = 0;
    float total_tat = 0;
    int last_event_time = 0;
    
    struct GanttBlock gantt[50];
    int ganttIndex = 0;

    for (i = 0; i < n; i++) {
        proc[i].completed = 0;
    }

    printf("\n--- SJF (Non-Preemptive) Execution Order ---\n");

    while (completed_processes < n) {
        shortest_job_index = -1;
        int min_bt = 9999;

        for (i = 0; i < n; i++) {
            if (proc[i].at <= current_time && proc[i].completed == 0) {
                if (proc[i].bt < min_bt) {
                    min_bt = proc[i].bt;
                    shortest_job_index = i;
                }
            }
        }

        if (shortest_job_index == -1) {
            current_time++;
        } else {
            i = shortest_job_index;
            
            // Check for idle time
            if (current_time > last_event_time) {
                printf("... CPU Idle from %d to %d ...\n", last_event_time, current_time);
                gantt[ganttIndex].pid = 0;
                gantt[ganttIndex].startTime = last_event_time;
                gantt[ganttIndex].endTime = current_time;
                ganttIndex++;
            }

            printf("Process P%d starts at time %d\n", proc[i].pid, current_time);
            
            gantt[ganttIndex].pid = proc[i].pid;
            gantt[ganttIndex].startTime = current_time;

            proc[i].ct = current_time + proc[i].bt;
            current_time = proc[i].ct;

            gantt[ganttIndex].endTime = current_time;
            ganttIndex++;

            proc[i].tat = proc[i].ct - proc[i].at;
            proc[i].wt = proc[i].tat - proc[i].bt;
            proc[i].completed = 1;
            completed_processes++;
            last_event_time = current_time;

            total_wt += proc[i].wt;
            total_tat += proc[i].tat;
        }
    }

    // Print the results table
    printf("\n--- SJF Scheduling Results ---\n");
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
    struct process proc[20];

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    printf("Enter Arrival Time and Burst Time for each process:\n");
    for (i = 0; i < n; i++) {
        printf("P%d: ", i + 1);
        proc[i].pid = i + 1;
        scanf("%d %d", &proc[i].at, &proc[i].bt);
    }

    findTimes(proc, n);

    return 0;
}
