#include <stdio.h>
#include <stdlib.h>

#define MAX_PAGES 20 // Maximum number of pages

int frames[20];     // Page frames
int frequency[20];  // Frequency of each page
int age[20];        // Age for FIFO tie-breaking

int max_frames;     // Number of frames

// Function to initialize arrays
void initialize() {
    for (int i = 0; i < max_frames; i++) {
        frames[i] = -1;      // Empty frame
        frequency[i] = 0;    // Reset frequency
        age[i] = 0;          // Reset age
    }
}

// Function to display frames
void displayFrames() {
    for (int i = 0; i < max_frames; i++) {
        if (frames[i] != -1)
            printf("%d ", frames[i]);
        else
            printf("- ");
    }
    printf("\n");
}

// Function to find the LFU page index with FIFO tie-breaking
int findLFU() {
    int minFreq = frequency[0];
    int minIndex = 0;

    for (int i = 1; i < max_frames; i++) {
        if (frequency[i] < minFreq) {
            minFreq = frequency[i];
            minIndex = i;
        } else if (frequency[i] == minFreq && age[i] < age[minIndex]) {
            minIndex = i;
        }
    }
    return minIndex;
}

// LFU Page Replacement Algorithm
void LFU(int pages[], int n) {
    int page_faults = 0, page_hits = 0, time = 0;

    for (int i = 0; i < n; i++) {
        int page = pages[i];
        int found = 0;

        // Check if page already in memory
        for (int j = 0; j < max_frames; j++) {
            if (frames[j] == page) {
                found = 1;
                frequency[j]++; // Increase frequency
                page_hits++;
                printf("Page %d → HIT\n", page);
                break;
            }
        }

        if (!found) {
            page_faults++;
            int replaceIndex = -1;

            // Check if any empty frame exists
            for (int j = 0; j < max_frames; j++) {
                if (frames[j] == -1) {
                    replaceIndex = j;
                    break;
                }
            }

            // If no empty frame, replace LFU page
            if (replaceIndex == -1) {
                replaceIndex = findLFU();
                printf("Page %d → FAULT (replaced page %d in frame %d)\n", page, frames[replaceIndex], replaceIndex);
            } else {
                printf("Page %d → FAULT (loaded into empty frame %d)\n", page, replaceIndex);
            }

            frames[replaceIndex] = page;
            frequency[replaceIndex] = 1; // Reset frequency
            age[replaceIndex] = time++;   // Update age for FIFO tie-breaking
        }

        displayFrames();
    }

    printf("\nTotal Page Faults: %d\n", page_faults);
    printf("Total Page Hits: %d\n", page_hits);
    printf("Hit Ratio: %.2f\n", (float)page_hits / n);
    printf("Miss Ratio: %.2f\n", (float)page_faults / n);
}

int main() {
    int pages[MAX_PAGES];
    int n;

    printf("Enter number of frames: ");
    scanf("%d", &max_frames);

    printf("Enter number of pages: ");
    scanf("%d", &n);

    printf("Enter the page reference sequence: ");
    for (int i = 0; i < n; i++) {
        scanf("%d", &pages[i]);
    }

    initialize();
    LFU(pages, n);

    return 0;
}
