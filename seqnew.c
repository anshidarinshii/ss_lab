#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BLOCKS 100
#define MAX_FILES 10

struct File
{
  char filename[50];
  int start_block;
  int num_block
} files[MAX_FILES];

void allocateContiguous(struct File* file,char* filename,int startBlock,int numBlocks,int disk[])
{
  strcpy(file->filename,filename);
  file->start_block=startBlock;
  file->num_block=numBlocks;

  for(int i=startBlock;i<startBlock+numBlocks;i++)
  {
    disk[i]=1;
  }
}

void main()
{
  int disk[MAX_BLOCKS]={0};
  int num_files;

  printf("Enter number of files to allocate (max %d): ", MAX_FILES);
  scanf("%d", &num_files);

  for(int i=0;i<num_files;i++)
  {
    char filename[50];
    int startBlock;
    int numBlocks;

    printf("\nFile %d:\n", i + 1);
    printf("Enter filename: ");
    scanf("%s", filename);

    printf("Enter start block (0-%d): ", MAX_BLOCKS - 1);
    scanf("%d", &startBlock);

    printf("Enter number of blocks: ");
    scanf("%d", &numBlocks);

    int overlap=0;
    for(int i=startBlock;i<startBlock+numBlocks;i++)
    {
      if (disk[i]==1)
      {
        overlap=1;
        break;
      }
    }

    if (overlap==1)
    {
      printf("Error: Blocks already allocated! Cannot allocate file %s.\n", filename);
      i--;
      continue;
    }

    allocateContiguous(&files[i],filename,startBlock,numBlocks,disk);

  }

    printf("\nFile Allocation Table:\n");
    printf("%-15s %-15s %-15s\n", "File Name", "Start Block", "Number of Blocks");

    for (int i = 0; i < num_files; i++) {
        printf("%-15s %-15d %-15d\n",
               files[i].filename, files[i].start_block, files[i].num_block);
    }

}
