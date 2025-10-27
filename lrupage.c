#include <stdio.h>
#include <stdlib.h>

#define MAX_FRAMES 3  // Number of available memory frames
#define MAX_PAGES 20  // Maximum number of page references

int frames[MAX_FRAMES]; // Array to store pages in memory
int last_used[MAX_FRAMES]; // Array to track last usage time

// Function to initialize frames and last_used arrays
void initialize()
{
    for (int i = 0; i < MAX_FRAMES; i++)
    {
        frames[i] = -1;  // Set all frames as empty
        last_used[i] = -1; // Initialize last usage time
    }
}

// Function to display the frames
void displayFrames()
{
    for (int i = 0; i < MAX_FRAMES; i++)
    {
        if (frames[i] != -1)
            printf("%d ", frames[i]);  // Print stored pages
        else
            printf("- ");  // Empty frame
    }
    printf("\n");
}

// Function to find the least recently used (LRU) page
int findLRU()
{
    int lru_index = 0;
    
    for (int i = 1; i < MAX_FRAMES; i++)
    {
        if (last_used[i] < last_used[lru_index]) // Find the oldest page
            lru_index = i;
    }
    
    return lru_index;
}

// Function to implement LRU Page Replacement Algorithm
void LRU(int pages[], int n)
{
    int page_faults = 0, time = 0;

    for (int i = 0; i < n; i++)
    {
        int page = pages[i];
        int found = 0;

        // Check if page is already in memory
        for (int j = 0; j < MAX_FRAMES; j++)
        {
            if (frames[j] == page)
            {
                found = 1;
                last_used[j] = time++;  // Update last used time
                printf("Page %d is already in memory.\n", page);
                break;
            }
        }

        if (!found)
        {
            int replaceIndex = findLRU(); // Find the LRU page index
            frames[replaceIndex] = page;  // Replace it with the new page
            last_used[replaceIndex] = time++; // Update usage time
            page_faults++;

            printf("Page %d loaded into frame %d.\n", page, replaceIndex);
        }

        displayFrames();
    }

    printf("Total Page Faults: %d\n", page_faults);
}

int main()
{
    int pages[MAX_PAGES];
    int n;

    printf("Enter number of pages: ");
    scanf("%d", &n);

    printf("Enter the page reference sequence: ");
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &pages[i]);
    }

    initialize();  // Reset all values
    LRU(pages, n); // Run LRU algorithm

    return 0;
}
