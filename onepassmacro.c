#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
    FILE *f1, *f2, *f3, *f5;
    int i, argCount = 0;
    char la[20], mne[20], opnd[50];
    char name[20], mne1[20], opnd1[50], argtab[10][20];

    f1 = fopen("inputm.txt", "r");
    f2 = fopen("namtab.txt", "w+");
    f3 = fopen("deftab.txt", "w+");
    f5 = fopen("op.txt", "w+");

    if (!f1) {
        printf(" Error: inputm.txt not found.\n");
        return 1;
    }

    fscanf(f1, "%s%s%s", la, mne, opnd);

    while (strcmp(mne, "END") != 0) {
        if (strcmp(mne, "MACRO") == 0) {
            char formal_params[10][20];
            int param_count = 0;
            
            fprintf(f2, "%s\n", la);
            rewind(f2);
            fprintf(f3, "%s\t%s\n", la, opnd);

            char temp_opnd[50];
            strcpy(temp_opnd, opnd);
            char *token = strtok(temp_opnd, ",");
            while (token != NULL) {
                strcpy(formal_params[param_count++], token);
                token = strtok(NULL, ",");
            }

            fscanf(f1, "%s%s%s", la, mne, opnd);
            while (strcmp(mne, "MEND") != 0) {
                if (opnd[0] == '&') {
                    for (int k = 0; k < param_count; k++) {
                        if (strcmp(opnd, formal_params[k]) == 0) {
                            sprintf(opnd, "?%d", k + 1);
                            break;
                        }
                    }
                }
                fprintf(f3, "%s\t%s\n", mne, opnd);
                fscanf(f1, "%s%s%s", la, mne, opnd);
            }
            fprintf(f3, "%s\n", mne);
        } else {
            rewind(f2);
            int found = 0;
            while (fscanf(f2, "%s", name) != EOF) {
                if (strcmp(mne, name) == 0) {
                    found = 1;
                    break;
                }
            }

            if (found) {
                argCount = 0;
                char *token = strtok(opnd, ",");
                while (token != NULL) {
                    strcpy(argtab[argCount++], token);
                    token = strtok(NULL, ",");
                }

                rewind(f3);
                while(fscanf(f3, "%s%s", mne1, opnd1) != EOF) {
                    if (strcmp(mne1, mne) == 0) {
                        break;
                    }
                }

                fprintf(f5, ".\t%s\t", mne);
                for (i = 0; i < argCount; i++) {
                    fprintf(f5, "%s%s", argtab[i], (i == argCount - 1) ? "" : ",");
                }
                fprintf(f5, "\n");

                while (fscanf(f3, "%s%s", mne1, opnd1) != EOF && strcmp(mne1, "MEND") != 0) {
                    if (opnd1[0] == '?') {
                        int index = opnd1[1] - '1';
                        if (index >= 0 && index < argCount) {
                            fprintf(f5, "-\t%s\t%s\n", mne1, argtab[index]);
                        }
                    } else {
                        fprintf(f5, "-\t%s\t%s\n", mne1, opnd1);
                    }
                }
            } else {
                fprintf(f5, "%s\t%s\t%s\n", la, mne, opnd);
            }
        }
        fscanf(f1, "%s%s%s", la, mne, opnd);
    }

    fprintf(f5, "%s\t%s\t%s\n", la, mne, opnd);
    fclose(f1);
    fclose(f2);
    fclose(f3);
    fclose(f5);

    printf("\n Successfully implemented Single Pass Macroprocessor.\n");

    char *filenames[] = {"inputm.txt", "namtab.txt", "deftab.txt", "op.txt"};
    char buffer[200];
    for (i = 0; i < 4; i++) {
        FILE *fp = fopen(filenames[i], "r");
        if (!fp) continue;
        printf("\n========== %s ==========\n", filenames[i]);
        while (fgets(buffer, sizeof(buffer), fp) != NULL) {
            printf("%s", buffer);
        }
        fclose(fp);
    }

    return 0;
}
