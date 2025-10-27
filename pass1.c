/*
 * C Program to Simulate Pass 1 of a Two-Pass Assembler
 *
 * This program reads from:
 * 1. input.txt  (Source Code)
 * 2. optab.txt   (Opcode Table)
 *
 * It produces:
 * 1. intermediate.txt (Source with addresses)
 * 2. symtab.txt       (Symbol Table)
 * 3. length.txt       (Program Length)
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Function to search OPTAB
// Returns 1 if found (and sets instruction length), 0 if not found
int search_optab(FILE *optab, const char *opcode, int *instr_length) {
    char op[20], len_str[10];
    rewind(optab); // Start search from the beginning of OPTAB

    while (fscanf(optab, "%s %s", op, len_str) != EOF) {
        if (strcmp(opcode, op) == 0) {
            *instr_length = atoi(len_str);
            return 1;
        }
    }
    return 0;
}

// Function to search SYMTAB
// Returns 1 if found, 0 if not found
int search_symtab(FILE *symtab, const char *label) {
    char sym[20], addr_str[10];
    rewind(symtab); // Start search from the beginning of SYMTAB

    while (fscanf(symtab, "%s %s", sym, addr_str) != EOF) {
        if (strcmp(label, sym) == 0) {
            return 1; // Found
        }
    }
    return 0;
}

int main() {
    FILE *f_input, *f_optab, *f_inter, *f_symtab, *f_length;
    int locctr, start_addr, instr_length, prog_length;
    char label[20], opcode[20], operand[20];
    int error_flag = 0;

    // 1. Open all files
    f_input = fopen("input.txt", "r");
    f_optab = fopen("optab.txt", "r");
    f_inter = fopen("intermediate.txt", "w");
    f_symtab = fopen("symtab.txt", "w");
    f_length = fopen("length.txt", "w");

    if (f_input == NULL || f_optab == NULL) {
        printf("Error: Cannot open input.txt or optab.txt\n");
        return 1;
    }

    // 2. Read First Line and Handle START
    fscanf(f_input, "%s %s %s", label, opcode, operand);

    if (strcmp(opcode, "START") == 0) {
        start_addr = (int)strtol(operand, NULL, 16); // Convert hex string to int
        locctr = start_addr;
        fprintf(f_inter, "%X\t%s\t%s\t%s\n", locctr, label, opcode, operand);
        fscanf(f_input, "%s %s %s", label, opcode, operand); // Read next line
    } else {
        start_addr = 0;
        locctr = 0;
    }

    // 3. Main Processing Loop (while opcode is not END)
    while (strcmp(opcode, "END") != 0) {
        
        // A. Write line to intermediate file
        fprintf(f_inter, "%X\t%s\t%s\t%s\n", locctr, label, opcode, operand);

        // B. Handle Label
        if (strcmp(label, "-") != 0) { // If a label exists
            if (search_symtab(f_symtab, label)) {
                printf("ERROR: Duplicate symbol '%s' at %X\n", label, locctr);
                error_flag = 1;
            } else {
                fprintf(f_symtab, "%s\t%X\n", label, locctr); // Add to SYMTAB
            }
        }

        // C. Process Opcode and increment LOCCTR
        if (search_optab(f_optab, opcode, &instr_length)) {
            locctr += instr_length;
        } else if (strcmp(opcode, "WORD") == 0) {
            locctr += 3;
        } else if (strcmp(opcode, "RESW") == 0) {
            locctr += (3 * atoi(operand));
        } else if (strcmp(opcode, "RESB") == 0) {
            locctr += atoi(operand);
        } else if (strcmp(opcode, "BYTE") == 0) {
            // C'EOF' -> 3 bytes
            // X'F1'  -> 1 byte
            if (operand[0] == 'C') {
                locctr += (strlen(operand) - 3); // -3 for C' '
            } else if (operand[0] == 'X') {
                locctr += (strlen(operand) - 3) / 2; // /2 for X' '
            }
        } else {
            printf("ERROR: Invalid opcode '%s' at %X\n", opcode, locctr);
            error_flag = 1;
        }

        // D. Read next line
        fscanf(f_input, "%s %s %s", label, opcode, operand);
    }

    // 4. Handle END directive
    fprintf(f_inter, "%X\t%s\t%s\t%s\n", locctr, label, opcode, operand);

    // 5. Calculate Program Length
    prog_length = locctr - start_addr;
    fprintf(f_length, "%X\n", prog_length); // Write length in hex
    printf("\nPass 1 complete.\n");
    printf("Program Length: %X\n", prog_length);

    if (error_flag) {
        printf("Errors found. Check output.\n");
    } else {
        printf("Intermediate file and SYMTAB created successfully.\n");
    }

    // 6. Close all files
    fclose(f_input);
    fclose(f_optab);
    fclose(f_inter);
    fclose(f_symtab);
    fclose(f_length);

    return 0;
}
