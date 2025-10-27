#include <stdio.h>
#include <stdlib.h>

#define MAX_PAGES 20

int frames[10];      // Array to store pages in memory
int last_used[10];   // Array to track last usage time

// Function to initialize frames and last_used arrays
void initialize(int frameCount) {
    for (int i = 0; i < frameCount; i++) {
        frames[i] = -1;
        last_used[i] = -1;
    }
}

// Function to display frames
void displayFrames(int frameCount) {
    for (int i = 0; i < frameCount; i++) {
        if (frames[i] != -1)
            printf("%d ", frames[i]);
        else
            printf("- ");
    }
    printf("\n");
}

// Function to find the least recently used (LRU) page index
int findLRU(int frameCount) {
    int lru_index = 0;
    for (int i = 1; i < frameCount; i++) {
        if (last_used[i] < last_used[lru_index])
            lru_index = i;
    }
    return lru_index;
}

// LRU Page Replacement Algorithm
void LRU(int pages[], int n, int frameCount) {
    int page_faults = 0, page_hits = 0, time = 0;

    printf("\n--- LRU Page Replacement ---\n");

    for (int i = 0; i < n; i++) {
        int page = pages[i];
        int found = 0;

        // Check if page is already in memory (HIT)
        for (int j = 0; j < frameCount; j++) {
            if (frames[j] == page) {
                found = 1;
                last_used[j] = time++;  // Update usage time
                page_hits++;
                printf("Page %d → HIT\n", page);
                break;
            }
        }

        // If page is not found (FAULT)
        if (!found) {
            int replaceIndex = findLRU(frameCount);
            frames[replaceIndex] = page;
            last_used[replaceIndex] = time++;
            page_faults++;
            printf("Page %d → FAULT (loaded into frame %d)\n", page, replaceIndex);
        }

        displayFrames(frameCount);
    }

    float hit_ratio = (float)page_hits / n;
    float miss_ratio = (float)page_faults / n;

    printf("\nTotal Page Faults = %d\n", page_faults);
    printf("Total Page Hits = %d\n", page_hits);
    printf("Hit Ratio = %.2f\n", hit_ratio);
    printf("Miss Ratio = %.2f\n", miss_ratio);
}

int main() {
    int pages[MAX_PAGES], n, frameCount;

    printf("Enter number of frames: ");
    scanf("%d", &frameCount);

    printf("Enter number of pages: ");
    scanf("%d", &n);

    printf("Enter the page reference sequence: ");
    for (int i = 0; i < n; i++)
        scanf("%d", &pages[i]);

    initialize(frameCount);
    LRU(pages, n, frameCount);

    return 0;
}
