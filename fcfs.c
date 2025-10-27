#include <stdio.h>
#include <stdlib.h>

struct Process
{
  int name;
  int at;
  int bt;
  int ct;
  int tat;
  int wt;
} processes[100];

void sortProcesses(struct Process processes[], int n)
{
  for (int i = 0; i < n - 1; i++)
  {
    for (int j = 0; j < n - i - 1; j++)
    {
      if (processes[j].at > processes[j + 1].at)
      {
        struct Process temp = processes[j];
        processes[j] = processes[j + 1];
        processes[j + 1] = temp;
      }
    }
  }
}

int main()
{
  int n;
  printf("Enter number of processes: ");
  scanf("%d", &n);

  for (int i = 0; i < n; i++)
  {
    processes[i].name = i + 1;
    printf("Enter arrival and burst time for P%d: ", i + 1);
    scanf("%d%d", &processes[i].at, &processes[i].bt);
  }

  int curtt = 0;
  sortProcesses(processes, n);

  for (int i = 0; i < n; i++)
  {
    if (curtt < processes[i].at)
      curtt = processes[i].at;

    curtt += processes[i].bt;
    processes[i].ct = curtt;
    processes[i].tat = processes[i].ct - processes[i].at;
    processes[i].wt = processes[i].tat - processes[i].bt;
  }

  printf("\nGantt Chart:\n");
  for (int i = 0; i < n; i++)
  {
    printf("|\tP%d\t", processes[i].name);
  }
  printf("|\n");

  printf("%d\t", processes[0].at < 0 ? 0 : processes[0].at);
  for (int i = 0; i < n; i++)
  {
    printf("\t%d\t", processes[i].ct);
  }
  printf("\n");

  float avgtat = 0.0;
  float avgwt = 0.0;

  printf("\nProcess  ArrivalTime BurstTime TurnaroundTime    WaitingTime   CompletionTime\n");
  for (int i = 0; i < n; i++)
  {
    printf("P%d%10d%10d%15d%20d%20d\n", processes[i].name, processes[i].at, processes[i].bt, processes[i].tat, processes[i].wt, processes[i].ct);
    avgwt = avgwt + processes[i].wt;
    avgtat = avgtat + processes[i].tat;
  }

  printf("\nAverage Waiting Time = %f\n", avgwt / n);
  printf("Average Turnaround Time = %f\n", avgtat / n);

  return 0;
}
