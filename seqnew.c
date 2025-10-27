#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BLOCKS 100
#define MAX_FILES 10

struct File {
    char filename[50];
    int start_block;
    int num_block;
} files[MAX_FILES];

void allocateContiguous(struct File* file, char* filename, int startBlock, int numBlocks, int disk[]) {
    strcpy(file->filename, filename);
    file->start_block = startBlock;
    file->num_block = numBlocks;

    for (int i = startBlock; i < startBlock + numBlocks; i++) {
        disk[i] = 1;  // mark blocks as allocated
    }
}

int main() {
    int disk[MAX_BLOCKS] = {0};
    int num_files;

    printf("Enter number of files to allocate (max %d): ", MAX_FILES);
    scanf("%d", &num_files);

    if (num_files <= 0 || num_files > MAX_FILES) {
        printf("❌ Invalid number of files!\n");
        return 1;
    }

    for (int i = 0; i < num_files; i++) {
        char filename[50];
        int startBlock, numBlocks;

        printf("\n📁 File %d:\n", i + 1);
        printf("Enter filename: ");
        scanf("%s", filename);

        printf("Enter start block (0–%d): ", MAX_BLOCKS - 1);
        scanf("%d", &startBlock);

        printf("Enter number of blocks: ");
        scanf("%d", &numBlocks);

        if (startBlock < 0 || startBlock >= MAX_BLOCKS || numBlocks <= 0 || startBlock + numBlocks > MAX_BLOCKS) {
            printf("❌ Invalid range of blocks!\n");
            i--;  // repeat current file input
            continue;
        }

        int overlap = 0;
        for (int b = startBlock; b < startBlock + numBlocks; b++) {
            if (disk[b] == 1) {
                overlap = 1;
                break;
            }
        }

        if (overlap == 1) {
            printf("⚠ Error: Some blocks already allocated! Cannot allocate file %s.\n", filename);
            i--;  // allow re-entry
            continue;
        }

        allocateContiguous(&files[i], filename, startBlock, numBlocks, disk);
        printf("✅ File %s allocated successfully from block %d to %d\n", filename, startBlock, startBlock + numBlocks - 1);
    }

    printf("\n================ FILE ALLOCATION TABLE ================\n");
    printf("%-15s %-15s %-15s\n", "File Name", "Start Block", "No. of Blocks");
    printf("-------------------------------------------------------\n");

    for (int i = 0; i < num_files; i++) {
        printf("%-15s %-15d %-15d\n", files[i].filename, files[i].start_block, files[i].num_block);
    }

    // Optional disk view
    printf("\nDisk Block Status (1=Allocated, 0=Free):\n");
    for (int i = 0; i < MAX_BLOCKS; i++) {
        printf("%d ", disk[i]);
        if ((i + 1) % 20 == 0) printf("\n"); // print 20 blocks per line
    }

    return 0;
}
