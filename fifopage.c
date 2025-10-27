#include <stdio.h>
#include <stdlib.h>

#define MAX_FRAMES 3 // Maximum number of frames
#define MAX_PAGES 20 // Maximum number of pages

int frames[MAX_FRAMES]; // Array to store the frames
int rear = -1;          // Pointer to the rear of the frame queue

// Function to initialize frames array
void initialize()
{
  for (int i = 0; i < MAX_FRAMES; i++)
  {
    frames[i] = -1; // Initializing frames with -1 (empty)
  }
}

// Function to display the frames
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

// Function to implement FIFO page replacement algorithm
void FIFO(int pages[], int n)
{
  int page_faults = 0;
  int front = 0; // Pointer to the front of the frame queue

  for (int i = 0; i < n; i++)
  {
    int page = pages[i];
    int found = 0;

    // Check if page already exists in frames
    for (int j = 0; j < MAX_FRAMES; j++)
    {
      if (frames[j] == page)
      {
        found = 1;
        printf("page %d already there\n", page);
        break;
      }
    }

    if (!found)
    {
      page_faults++;

      if (rear < MAX_FRAMES - 1)
      {
        rear++;
      }
      else
      {
        rear = 0;
      }
      frames[rear] = page;
      printf("page %d loaded in frame %d \n", page, rear);
    }

    // printf("Page %d : ", page);
    displayFrames();
  }

  printf("Total Page Faults: %d\n", page_faults);
}

int main()
{
  int pages[MAX_PAGES]; // Array to store pages

  // Example sequence of page references
  int n;
  printf("Enter number of pages: ");
  scanf("%d", &n);
  printf("Enter the page reference sequence: ");
  for (int i = 0; i < n; i++)
  {
    scanf("%d", &pages[i]);
  }

  initialize();   // Initialize frames array
  FIFO(pages, n); // Apply FIFO algorithm

  return 0;
}
