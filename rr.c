/*
 * C Program for Round Robin (RR) Scheduling
 *
 * Calculates CT, TAT, WT, and displays Gantt Chart.
 */

#include <stdio.h>

// Define a structure for a process
struct process {
    int pid;
    int at;
    int bt;
    int rt;  // Remaining Time
    int ct;
    int tat;
    int wt;
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
void findTimes(struct process proc[], int n, int tq) {
    int i;
    int current_time = 0;
    int completed_processes = 0;
    float total_wt = 0;
    float total_tat = 0;
    int last_event_time = 0;

    struct GanttBlock gantt[50];
    int ganttIndex = 0;

    for (i = 0; i < n; i++) {
        proc[i].rt = proc[i].bt;
    }

    printf("\n--- Round Robin (TQ=%d) Execution Order ---\n", tq);

    while (completed_processes < n) {
        int process_ran_this_cycle = 0;

        for (i = 0; i < n; i++) {
            if (proc[i].at <= current_time && proc[i].rt > 0) {
                process_ran_this_cycle = 1;

                // Check for idle time
                if (current_time > last_event_time) {
                    printf("... CPU Idle from %d to %d ...\n", last_event_time, current_time);
                    gantt[ganttIndex].pid = 0;
                    gantt[ganttIndex].startTime = last_event_time;
                    gantt[ganttIndex].endTime = current_time;
                    ganttIndex++;
                }

                gantt[ganttIndex].pid = proc[i].pid;
                gantt[ganttIndex].startTime = current_time;

                if (proc[i].rt > tq) {
                    // Process runs for Time Quantum
                    printf("Process P%d runs from %d to %d\n", proc[i].pid, current_time, current_time + tq);
                    current_time += tq;
                    proc[i].rt -= tq;
                    
                    gantt[ganttIndex].endTime = current_time;
                } else {
                    // Process runs for its remaining time (and finishes)
                    printf("Process P%d runs from %d to %d (Finishes)\n", proc[i].pid, current_time, current_time + proc[i].rt);
                    current_time += proc[i].rt;
                    proc[i].rt = 0;
                    
                    gantt[ganttIndex].endTime = current_time;
                    
                    proc[i].ct = current_time;
                    completed_processes++;

                    proc[i].tat = proc[i].ct - proc[i].at;
                    proc[i].wt = proc[i].tat - proc[i].bt;

                    total_wt += proc[i].wt;
                    total_tat += proc[i].tat;
                }
                
                ganttIndex++;
                last_event_time = current_time;
            }
        }

        // If no process ran, CPU must be idle
        if (process_ran_this_cycle == 0) {
            // Check if we are just waiting for an arrival
            int all_arrived = 1;
            for(i=0; i<n; i++) {
                if(proc[i].at > current_time) {
                    all_arrived = 0;
                    // To avoid large idle blocks, advance time to next arrival
                    // This is a simple way:
                    current_time++; 
                    break;
                }
            }
            if(all_arrived) {
                 // This shouldn't be hit if logic is correct, but as a safeguard.
                 break;
            }
        }
    }

    // Print the results table
    printf("\n--- Round Robin Scheduling Results ---\n");
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
    int n, i, tq;
    struct process proc[20];

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    printf("Enter the Time Quantum (TQ): ");
    scanf("%d", &tq);

    printf("Enter Arrival Time and Burst Time for each process:\n");
    for (i = 0; i < n; i++) {
        printf("P%d: ", i + 1);
        proc[i].pid = i + 1;
        scanf("%d %d", &proc[i].at, &proc[i].bt);
    }

    findTimes(proc, n, tq);

    return 0;
}
