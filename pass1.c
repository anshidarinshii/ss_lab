#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char opcode[15],operand[15],label[30];
  char optab_opcode[15],optab_value[15];
  int locctr, start,length;
  FILE  *inp,*symtab, *optab, *inter, *len;
  inp = fopen("input.txt","r");
  symtab = fopen("symtab.txt","w");
  optab  = fopen("optab.txt","r");
  inter = fopen("intermediate.txt","w");
  len = fopen("length.txt","w");

  if (!inp || !symtab ||!optab || !inter || !len) {
    printf("Error opening file\n");
    exit(1);
  }

  fscanf(inp,"%s %s %s",label,opcode,operand);
  if (strcmp(opcode,"START") == 0) {
    locctr = (int)strtol(operand,NULL,16);
    start = locctr;
    fprintf(inter,"%-8X%-8s%-8s%s\n",locctr,label,opcode,operand);
    fscanf(inp,"%s %s %s",label,opcode,operand);
  } else {
    locctr = start = 0;
  }

  while(strcmp(opcode,"END") != 0){
    if (strcmp(opcode,"~") != 0) {
      fprintf(symtab,"%-8s%04X\n",label,locctr);
    }
    fprintf(inter,"%-8X%-8s%-8s%s\n",locctr,label,opcode,operand);
    rewind(optab);
    int found = 0;
    while(fscanf(optab, "%s %s",optab_opcode,optab_value) == 2) {
      if (strcmp(opcode,optab_opcode) == 0) {
        locctr +=3;
        found =1;
        break;
      }
      if (!found){
        if(strcmp(opcode,"WORD")) {
          locctr +=3;
        } else if (strcmp(opcode,"RESW")) {
          locctr +=3 *atoi(operand);
        } else if (strcmp(opcode,"RESB")){
          locctr += atoi(operand);
        } else if(strcmp(opcode,"BYTE")) {
          if (operand[0] == 'C') {
            locctr += strlen(operand) -3;
          } else if (operand[0] == 'X'){
            locctr += (strlen(operand)- 2)/2;
          }
        }
      }
    }
    fscanf(inp,"%s %s %s",label,opcode,operand);
  }
  fprintf(inter,"%-8X%-8s%-8s%s\n",locctr,label,opcode,operand);
  length = locctr - start;
  fprintf(len,"%X",length);
  fclose(inp);
  fclose(symtab);
  fclose(optab);
  fclose(inter);
  fclose(len);
  return 0;
}
