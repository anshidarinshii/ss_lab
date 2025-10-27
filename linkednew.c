#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BLOCKS 100
#define MAX_FILES 10
#define MAX_FILENAME 50

struct Block
{
  int block_number;
  struct Block *next;
};

struct File
{
  char filename[MAX_FILENAME];
  struct Block *head;
};

int blockAllocated[MAX_BLOCKS] = {0};

struct Block *allocateBlock(int blockNumber)
{
  struct Block *newBlock = (struct Block *)malloc(sizeof(struct Block));
  if (!newBlock)
  {
    printf("Memory allocation failed\n");
    exit(1);
  }
  newBlock->block_number = blockNumber;
  newBlock->next = NULL;
  return newBlock;
}

void freeBlocks(struct Block *head)
{
  struct Block *current = head;
  while (current)
  {
    struct Block *temp = current;
    current = current->next;
    free(temp);
  }
}

int main()
{
  struct File files[MAX_FILES];
  int num_files;

  printf("Enter number of files to allocate (max %d): ", MAX_FILES);
  scanf("%d", &num_files);

  if (num_files <= 0 || num_files > MAX_FILES)
  {
    printf("Invalid number of files.\n");
    return 1;
  }

  for (int i = 0; i < num_files; i++)
  {
    printf("\nFile %d:\n", i + 1);
    printf("Enter filename: ");
    scanf("%s", files[i].filename);

    int num_blocks;
    printf("Enter number of blocks to allocate: ");
    scanf("%d", &num_blocks);

    if (num_blocks <= 0 || num_blocks > MAX_BLOCKS)
    {
      printf("Invalid number of blocks.\n");
      i--;
      continue;
    }

    struct Block *head = NULL;
    struct Block *current = NULL;

    for (int j = 0; j < num_blocks; j++)
    {
      int block_num;
      printf("Enter block number %d: ", j + 1);
      scanf("%d", &block_num);

      if (block_num < 0 || block_num >= MAX_BLOCKS)
      {
        printf(" Invalid block number! Must be between 0 and %d.\n", MAX_BLOCKS - 1);
        j--;
        continue;
      }

      if (blockAllocated[block_num])
      {
        printf(" Block %d is already allocated to another file! Choose a different block.\n", block_num);
        j--;
        continue;
      }

      blockAllocated[block_num] = 1;

      struct Block *newBlock = allocateBlock(block_num);
      if (head == NULL)
      {
        head = newBlock;
        current = head;
      }
      else
      {
        current->next = newBlock;
        current = newBlock;
      }
    }
    files[i].head = head;
  }

  printf("\nAllocated files and their blocks:\n");
  for (int i = 0; i < num_files; i++)
  {
    printf("File: %s\nBlocks: ", files[i].filename);
    struct Block *current = files[i].head;
    while (current)
    {
      printf("%d ", current->block_number);
      current = current->next;
    }
    printf("\n\n");
  }

  for (int i = 0; i < num_files; i++)
  {
    freeBlocks(files[i].head);
  }

  return 0;
}
