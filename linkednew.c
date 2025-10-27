#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BLOCKS 100
#define MAX_FILES 10
#define MAX_FILENAME 50

// Structure for each data block
struct Block {
    int block_number;
    struct Block *next;
};

// Structure for file details
struct File {
    char filename[MAX_FILENAME];
    struct Block *head;
};

// To track allocated disk blocks (0 = free, 1 = allocated)
int blockAllocated[MAX_BLOCKS] = {0};

// Function to create a new block node
struct Block *allocateBlock(int blockNumber) {
    struct Block *newBlock = (struct Block *)malloc(sizeof(struct Block));
    if (!newBlock) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    newBlock->block_number = blockNumber;
    newBlock->next = NULL;
    return newBlock;
}

// Function to free allocated blocks
void freeBlocks(struct Block *head) {
    struct Block *current = head;
    while (current) {
        struct Block *temp = current;
        current = current->next;
        free(temp);
    }
}

int main() {
    struct File files[MAX_FILES];
    int num_files;

    printf("Enter number of files to allocate (max %d): ", MAX_FILES);
    scanf("%d", &num_files);

    if (num_files <= 0 || num_files > MAX_FILES) {
        printf("Invalid number of files!\n");
        return 1;
    }

    // Process each file
    for (int i = 0; i < num_files; i++) {
        printf("\nFile %d:\n", i + 1);
        printf("Enter filename: ");
        scanf("%s", files[i].filename);

        int num_blocks;
        printf("Enter number of blocks to allocate: ");
        scanf("%d", &num_blocks);

        if (num_blocks <= 0 || num_blocks > MAX_BLOCKS) {
            printf("Invalid number of blocks!\n");
            i--;
            continue;
        }

        struct Block *head = NULL;
        struct Block *current = NULL;

        // Allocate blocks
        for (int j = 0; j < num_blocks; j++) {
            int blockNumber;
            printf("Enter block number %d: ", j + 1);
            scanf("%d", &blockNumber);

            if (blockNumber < 0 || blockNumber >= MAX_BLOCKS) {
                printf("❌ Invalid block number! Must be between 0 and %d.\n", MAX_BLOCKS - 1);
                j--;
                continue;
            }

            if (blockAllocated[blockNumber]) {
                printf("⚠ Block %d already allocated! Choose a different block.\n", blockNumber);
                j--;
                continue;
            }

            blockAllocated[blockNumber] = 1;

            struct Block *newBlock = allocateBlock(blockNumber);

            if (head == NULL) {
                head = newBlock;
                current = head;
            } else {
                current->next = newBlock;
                current = newBlock;
            }
        }

        files[i].head = head;
    }

    // Display all file allocations
    printf("\n==============================\n");
    printf("  FILE ALLOCATION TABLE\n");
    printf("==============================\n");
    for (int i = 0; i < num_files; i++) {
        printf("File: %-10s | Blocks: ", files[i].filename);
        struct Block *current = files[i].head;
        while (current) {
            printf("%d ", current->block_number);
            current = current->next;
        }
        printf("\n");
    }
    printf("==============================\n");

    // Free all allocated memory
    for (int i = 0; i < num_files; i++) {
        freeBlocks(files[i].head);
    }

    return 0;
}
