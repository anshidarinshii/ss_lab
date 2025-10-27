#include <stdio.h>
#include <stdlib.h>

#define DISK_SIZE 200 // Assuming disk tracks range from 0 to 199

int main()
{
    int n, total_move = 0, initial, direction;

    
    printf("Enter the number of disk requests: ");
    scanf("%d", &n);

    if (n <= 0)
    {
        printf("Invalid number of requests.\n");
        return 1;
    }

    int rs[n];
    printf("Enter the request sequence:\n");
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &rs[i]);
    }

    printf("Enter the initial head position: ");
    scanf("%d", &initial);

    if (initial < 0 || initial >= DISK_SIZE)
    {
        printf("Invalid initial head position.\n");
        return 1;
    }

    printf("Enter the direction for movement (0 for left, 1 for right): ");
    scanf("%d", &direction);

    int left[n], right[n], left_size = 0, right_size = 0;

    for (int i = 0; i < n; i++)
    {
        if (rs[i] < initial)
        {
            left[left_size++] = rs[i];
        }
        else
        {
            right[right_size++] = rs[i];
        }
    }

    // Sorting both halves
    for (int i = 0; i < left_size - 1; i++)
    {
        for (int j = 0; j < left_size - i - 1; j++)
        {
            if (left[j] > left[j + 1])
            {
                int temp = left[j];
                left[j] = left[j + 1];
                left[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < right_size - 1; i++)
    {
        for (int j = 0; j < right_size - i - 1; j++)
        {
            if (right[j] > right[j + 1])
            {
                int temp = right[j];
                right[j] = right[j + 1];
                right[j + 1] = temp;
            }
        }
    }

    printf("\nHead Movement Sequence:\n%d", initial); 


    if (direction == 1)
    { 
        for (int i = 0; i < right_size; i++)
        {
            total_move += abs(right[i] - initial);
            initial = right[i];
            printf(" -> %d", initial);
        }

   
        total_move += abs(DISK_SIZE - 1 - initial);
        initial = 0;
        printf(" -> %d", initial);

        for (int i = 0; i < left_size; i++)
        {
            total_move += abs(left[i] - initial);
            initial = left[i];
            printf(" -> %d", initial);
        }
    }
    else
    { 
        for (int i = left_size - 1; i >= 0; i--)
        {
            total_move += abs(left[i] - initial);
            initial = left[i];
            printf(" -> %d", initial);
        }

        
        total_move += abs(initial - 0);
        initial = DISK_SIZE - 1;
        printf(" -> %d", initial);

        
        for (int i = right_size - 1; i >= 0; i--)
        {
            total_move += abs(right[i] - initial);
            initial = right[i];
            printf(" -> %d", initial);
        }
    }


    printf("\n\nTotal seek time using C-SCAN: %d\n", total_move);

    return 0;
}
