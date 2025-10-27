#include <stdio.h>
#include <stdlib.h>

#define MAX_PAGES 30

void displayFrames(int frames[], int frameCount)
{
    for (int i = 0; i < frameCount; i++)
    {
        if (frames[i] == -1)
            printf("- ");
        else
            printf("%d ", frames[i]);
    }
    printf("\n");
}

void FIFO(int pages[], int n, int frameCount)
{
    int frames[frameCount];
    int page_faults = 0, page_hits = 0;
    int index = 0; // acts as the FIFO pointer

    // Initialize all frames to -1
    for (int i = 0; i < frameCount; i++)
        frames[i] = -1;

    printf("\n--- FIFO Page Replacement ---\n");

    for (int i = 0; i < n; i++)
    {
        int page = pages[i];
        int found = 0;

        // Check if page is already present
        for (int j = 0; j < frameCount; j++)
        {
            if (frames[j] == page)
            {
                found = 1;
                page_hits++;
                printf("Page %d → HIT → ", page);
                displayFrames(frames, frameCount);
                break;
            }
        }

        // If not found, replace oldest page
        if (!found)
        {
            frames[index] = page;
            index = (index + 1) % frameCount;
            page_faults++;

            printf("Page %d → FAULT → ", page);
            displayFrames(frames, frameCount);
        }
    }

    float hit_ratio = (float)page_hits / n;
    float miss_ratio = (float)page_faults / n;

    printf("\nTotal Page Faults = %d\n", page_faults);
    printf("Total Page Hits = %d\n", page_hits);
    printf("Hit Ratio = %.2f\n", hit_ratio);
    printf("Miss Ratio = %.2f\n", miss_ratio);
}

int main()
{
    int n, frameCount;
    int pages[MAX_PAGES];

    printf("Enter number of pages: ");
    scanf("%d", &n);

    printf("Enter the page reference sequence: ");
    for (int i = 0; i < n; i++)
        scanf("%d", &pages[i]);

    printf("Enter number of frames: ");
    scanf("%d", &frameCount);

    FIFO(pages, n, frameCount);

    return 0;
}
