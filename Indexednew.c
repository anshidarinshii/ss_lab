#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BLOCKS 100
#define MAX_FILES 10
#define MAX_FILENAME 50

int freeBlocks[MAX_BLOCKS];  // 0 = free, 1 = allocated

struct IndexBlock {
    int data_blocks[MAX_BLOCKS];
};

struct File {
    char filename[MAX_FILENAME];
    struct IndexBlock indexBlock;
};

// Initialize all blocks as free
void initializeFreeBlocks() {
    for (int i = 0; i < MAX_BLOCKS; i++) {
        freeBlocks[i] = 0;
    }
}

// Initialize index block entries as empty (-1)
void initializeIndexBlock(struct IndexBlock* indexBlock) {
    for (int i = 0; i < MAX_BLOCKS; i++) {
        indexBlock->data_blocks[i] = -1;
    }
}

// Allocate a data block to a file’s index block
int allocateDataBlock(struct IndexBlock* indexBlock, int blockNumber) {
    if (blockNumber < 0 || blockNumber >= MAX_BLOCKS) {
        printf("❌ Invalid block number %d\n", blockNumber);
        return 0;
    }

    if (freeBlocks[blockNumber] == 1) {
        printf("⚠ Block %d already allocated to another file!\n", blockNumber);
        return 0;
    }

    for (int i = 0; i < MAX_BLOCKS; i++) {
        if (indexBlock->data_blocks[i] == -1) {
            indexBlock->data_blocks[i] = blockNumber;
            freeBlocks[blockNumber] = 1;
            return 1;
        }
    }

    printf("❌ Index block is full. Cannot allocate more blocks.\n");
    return 0;
}

// Display file and its allocated blocks
void displayFileBlocks(struct File* file) {
    printf("\nFile: %s\nAllocated Blocks: ", file->filename);
    for (int i = 0; i < MAX_BLOCKS; i++) {
        if (file->indexBlock.data_blocks[i] != -1) {
            printf("%d ", file->indexBlock.data_blocks[i]);
        }
    }
    printf("\n");
}

// Main function
int main() {
    struct File files[MAX_FILES];
    int num_files;

    initializeFreeBlocks();

    printf("Enter number of files (max %d): ", MAX_FILES);
    scanf("%d", &num_files);

    if (num_files <= 0 || num_files > MAX_FILES) {
        printf("Invalid number of files.\n");
        return 1;
    }

    for (int i = 0; i < num_files; i++) {
        printf("\nFile %d:\n", i + 1);
        printf("Enter filename: ");
        scanf("%s", files[i].filename);

        initializeIndexBlock(&files[i].indexBlock);

        int num_blocks;
        printf("Enter number of blocks to allocate (max %d): ", MAX_BLOCKS);
        scanf("%d", &num_blocks);

        if (num_blocks <= 0 || num_blocks > MAX_BLOCKS) {
            printf("Invalid number of blocks.\n");
            i--;
            continue;
        }

        for (int j = 0; j < num_blocks; j++) {
            int block_num;
            printf("Enter block number %d: ", j + 1);
            scanf("%d", &block_num);

            if (!allocateDataBlock(&files[i].indexBlock, block_num)) {
                printf("Skipping this block allocation.\n");
                j--;
            }
        }
    }

    printf("\n===== Summary of Allocated Files and Their Blocks =====\n");
    for (int i = 0; i < num_files; i++) {
        displayFileBlocks(&files[i]);
    }

    return 0;
}
