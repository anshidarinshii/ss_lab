#include <stdio.h>
#include <stdlib.h>

#define MAX_FRAMES 3 // Maximum number of frames
#define MAX_PAGES 20 // Maximum number of pages

int frames[MAX_FRAMES]; // Page frames
int frequency[MAX_FRAMES]; // Frequency of each page
int age[MAX_FRAMES]; // Age for tie-breaking

// Function to initialize arrays
void initialize()
{
    for (int i = 0; i < MAX_FRAMES; i++)
    {
        frames[i] = -1;  // Empty frames
        frequency[i] = 0; // Reset frequency
        age[i] = 0;       // Reset age
    }
}

// Function to display frames
void displayFrames()
{
    for (int i = 0; i < MAX_FRAMES; i++)
    {
        if (frames[i] != -1)
            printf("%d ", frames[i]);
        else
            printf("- ");
    }
    printf("\n");
}

// Function to find the least frequently used page with FIFO tie-breaking
int findLFU()
{
    int minFreq = frequency[0];
    int minIndex = 0;

    for (int i = 1; i < MAX_FRAMES; i++)
    {
        // Replace if a page has lower frequency
        if (frequency[i] < minFreq)
        {
            minFreq = frequency[i];
            minIndex = i;
        }
        // If same frequency, use FIFO (oldest page gets replaced)
        else if (frequency[i] == minFreq && age[i] < age[minIndex])
        {
            minIndex = i;
        }
    }
    return minIndex;
}

// LFU Page Replacement Algorithm
void LFU(int pages[], int n)
{
    int page_faults = 0, time = 0;

    for (int i = 0; i < n; i++)
    {
        int page = pages[i];
        int found = 0;

        // Check if page exists in frames
        for (int j = 0; j < MAX_FRAMES; j++)
        {
            if (frames[j] == page)
            {
                found = 1;
                frequency[j]++; // Increase frequency count
                printf("Page %d is already in memory.\n", page);
                break;
            }
        }

        if (!found)
        {
            int replaceIndex = findLFU(); // Find LFU page index
            frames[replaceIndex] = page;
            frequency[replaceIndex] = 1; // Reset frequency for new page
            age[replaceIndex] = time++; // Update age for FIFO tie-breaking
            page_faults++;

            printf("Page %d loaded into frame %d.\n", page, replaceIndex);
        }

        displayFrames();
    }

    printf("Total Page Faults: %d\n", page_faults);
}

int main()
{
    int pages[MAX_PAGES]; // Page reference sequence
    int n;

    printf("Enter number of pages: ");
    scanf("%d", &n);

    printf("Enter the page reference sequence: ");
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &pages[i]);
    }

    initialize();  // Initialize arrays
    LFU(pages, n); // Run LFU algorithm

    return 0;
}
