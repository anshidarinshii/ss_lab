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

int shortestJob(struct Process processes[], int n, int curtt, int completed[])
{
  int minindex = -1;
  int minbt = 9999;
  for (int i = 0; i < n; i++)
  {
    if (!completed[i] && processes[i].at <= curtt && processes[i].bt < minbt)
    {
      minindex = i;
      minbt = processes[i].bt;
    }
  }
  return minindex;
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

  int completed[n];
  int compl = 0;
  for (int i = 0; i < n; i++)
    completed[i] = 0;

  // Arrays to keep order of execution
  int order[100], times[100], idx = 0;

  while (compl < n)
  {
    int i = shortestJob(processes, n, curtt, completed);

    if (i == -1)
    {
      curtt++;
      continue;
    }

    curtt += processes[i].bt;
    processes[i].ct = curtt;
    processes[i].tat = processes[i].ct - processes[i].at;
    processes[i].wt = processes[i].tat - processes[i].bt;
    completed[i] = 1;
    compl++;

    
    order[idx] = processes[i].name;
    times[idx] = processes[i].ct;
    idx++;
  }

  
  printf("\nGantt chart : \n");
  for (int i = 0; i < idx; i++)
  {
    printf("|\tP%d\t", order[i]);
  }
  printf("|\n");

  printf("0\t");
  for (int i = 0; i < idx; i++)
  {
    printf("\t%d\t", times[i]);
  }
  printf("\n");
  

  float avgtat = 0.0;
  float avgwt = 0.0;

  printf("\nProcess  ArrivalTime BurstTime TurnaroundTime    WaitingTime   CompletionTime\n");
  for (int i = 0; i < n; i++)
  {
    printf("P%d%10d%10d%15d%20d%20d\n", processes[i].name, processes[i].at,
           processes[i].bt, processes[i].tat, processes[i].wt, processes[i].ct);
    avgwt += processes[i].wt;
    avgtat += processes[i].tat;
  }

  printf("\nAverage Waiting Time = %f\n", avgwt / n);
  printf("Average Turnaround Time = %f\n", avgtat / n);

  return 0;
}
