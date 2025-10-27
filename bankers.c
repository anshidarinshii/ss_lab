#include<stdio.h>


#define P 5  
#define R 3 

void print(int m[][R])
{
    for(int i=0;i<P;i++)
    {
    for(int j=0;j<R;j++)
        printf("%d-",m[i][j]);
    printf("\n");
    }
}

int isSafe(int processes[], int available[], int max[][R], int allot[][R]) {
    int need[P][R];
    int finish[P] = {0};
    
  
    for (int i = 0; i < P; i++) {
        for (int j = 0; j < R; j++) {
            need[i][j] = max[i][j] - allot[i][j];
        }
    }
    
    printf("Resource need\n");
    print(need);
    int work[R];
    
    for (int i = 0; i < R; i++) {
        work[i] = available[i];
        
    }

    int count = 0;
    while (count < P) 
    {
        int found = 0; 
        for (int i = 0; i < P; i++) 
        {
            if (finish[i] == 0) 
            {
                int j,flag=1;
                for (j = 0; j < R; j++) 
                {
                    if (need[i][j] > work[j]) 
                    {
                        flag=0;break;
                    }
                }
                if (flag == 1) 
                {
                    
                   
                    printf("Available Resources");
                    for (int k = 0; k < R; k++)
                        {printf("-%d",work[k]);}
                        
                        printf("\nprocess-p%d can be allocated resources",i);
                    for (int k = 0; k < R; k++)
                        {printf("-%d",need[i][k]);}
                        
                       
                    
                    for (int k = 0; k < R; k++) 
                    {
                        work[k] += allot[i][k];
                        
                    }
                    printf("\n");
                    finish[i] = 1;
                    found = 1;
                    count++;
                }
            }
        }
        if (found == 0) 
        {
            
            return 0;
        }
    }
    
    return 1;
}


void bankerAlgorithm(int processes[], int available[], int max[][R], int allot[][R]) {
    if (isSafe(processes, available, max, allot)) {
        printf("Safe state, allocating resources...\n");
        
        
    } else {
        printf("Unsafe state, cannot allocate resources. Possible deadlock.\n");
    }
}


int main() {
    int processes[] = {0, 1, 2, 3, 4}; 
    int available[] = {3, 3, 2}; 

  
    int max[P][R] = {
        {7, 5, 3},
        {3, 2, 2},
        {9, 0, 2},
        {2, 2, 2},
        {4, 3, 3}
    };

    
    int allot[P][R] = {
        {0, 1, 0},
        {2, 0, 0},
        {3, 0, 2},
        {2, 1, 1},
        {0, 0, 2}
    };

    printf("Max Requirement\n");
    print(max);
    printf("Resource Allotment\n");
    print(allot);
    bankerAlgorithm(processes, available, max, allot);

    return 0;
}



