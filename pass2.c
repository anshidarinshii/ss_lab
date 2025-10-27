/*
 * C Program to Simulate Pass 2 of a Two-Pass Assembler
 *
 * This program reads from:
 * 1. intermediate.txt (From Pass 1)
 * 2. symtab.txt       (From Pass 1)
 * 3. length.txt       (From Pass 1)
 * 4. optab_pass2.txt  (New OPTAB with machine codes)
 *
 * It produces:
 * 1. listing.txt       (Final listing with object code)
 * 2. object_program.txt(Final H-T-E Object Program)
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Function to search OPTAB for machine code
// Returns 1 if found (and sets machine_code), 0 if not found
int search_optab(FILE *optab, const char *opcode, char *machine_code) {
    char op[20], code[10];
    rewind(optab); // Start from the beginning

    while (fscanf(optab, "%s %s", op, code) != EOF) {
        if (strcmp(opcode, op) == 0) {
            strcpy(machine_code, code);
            return 1;
        }
    }
    return 0;
}

// Function to search SYMTAB for address
// Returns 1 if found (and sets operand_addr), 0 if not found
int search_symtab(FILE *symtab, const char *label, char *operand_addr) {
    char sym[20], addr_str[10];
    rewind(symtab); // Start from the beginning

    while (fscanf(symtab, "%s %s", sym, addr_str) != EOF) {
        if (strcmp(label, sym) == 0) {
            strcpy(operand_addr, addr_str);
            // Ensure address is 4 digits, pad with 0 if needed
            if (strlen(operand_addr) < 4) {
                char temp[5] = "0000";
                strcpy(temp + (4 - strlen(operand_addr)), operand_addr);
                strcpy(operand_addr, temp);
            }
            return 1;
        }
    }
    return 0;
}

// Helper to format a string to 6 hex digits
void format_hex_6(char *dest, int num) {
    sprintf(dest, "%06X", num);
}

// Helper to format a string to 2 hex digits
void format_hex_2(char *dest, int num) {
    sprintf(dest, "%02X", num);
}

int main() {
    FILE *f_inter, *f_optab, *f_symtab, *f_length, *f_list, *f_obj;
    int start_addr, prog_length;
    char locctr_str[20], label[20], opcode[20], operand[20], prog_name[20];
    char machine_code[10], operand_addr[10], object_code[30];

    char t_record_buffer[70]; // Buffer for one T-record
    int t_record_len = 0;     // Current length of buffer (in chars)
    int t_start_addr = 0;     // Starting address of current T-record

    // 1. Open all files
    f_inter = fopen("intermediate.txt", "r");
    f_symtab = fopen("symtab.txt", "r");
    f_optab = fopen("optab_pass2.txt", "r");
    f_length = fopen("length.txt", "r");
    f_list = fopen("listing.txt", "w");
    f_obj = fopen("object_program.txt", "w");

    if (!f_inter || !f_symtab || !f_optab || !f_length) {
        printf("Error: Cannot open input files (intermediate, symtab, optab, length)\n");
        return 1;
    }

    // 2. Read Program Length
    fscanf(f_length, "%X", &prog_length);
    fclose(f_length);

    // 3. Read First Line (Handle START)
    fscanf(f_inter, "%s %s %s %s", locctr_str, label, opcode, operand);
    if (strcmp(opcode, "START") == 0) {
        start_addr = (int)strtol(operand, NULL, 16);
        strcpy(prog_name, label);
        fprintf(f_list, "%s\t%s\t%s\t%s\t-\n", locctr_str, label, opcode, operand);
    } else {
        start_addr = 0;
        strcpy(prog_name, "DEFAULT");
        // Rewind if no START, process this line in the loop
        rewind(f_inter);
    }

    // 4. Write Header (H) Record
    fprintf(f_obj, "H^%-6s^%06X^%06X\n", prog_name, start_addr, prog_length);

    // 5. Main Processing Loop (while opcode is not END)
    t_start_addr = start_addr;
    t_record_buffer[0] = '\0'; // Initialize buffer as empty
    t_record_len = 0;

    while (fscanf(f_inter, "%s %s %s %s", locctr_str, label, opcode, operand) != EOF) {
        
        if (strcmp(opcode, "END") == 0) {
            fprintf(f_list, "%s\t%s\t%s\t%s\t-\n", locctr_str, label, opcode, operand);
            break;
        }

        strcpy(object_code, ""); // Reset object code for this line

        // C. Process Opcode
        if (search_optab(f_optab, opcode, machine_code)) {
            // Instruction
            if (strcmp(operand, "-") != 0) {
                if (!search_symtab(f_symtab, operand, operand_addr)) {
                    printf("ERROR: Undefined symbol '%s'\n", operand);
                    strcpy(operand_addr, "0000"); // Use 0000 for error
                }
            } else {
                strcpy(operand_addr, "0000"); // No operand (e.g., RSUB)
            }
            sprintf(object_code, "%s%s", machine_code, operand_addr);

        } else if (strcmp(opcode, "WORD") == 0) {
            // Data constant
            format_hex_6(object_code, atoi(operand));

        } else if (strcmp(opcode, "BYTE") == 0) {
            // Data constant
            if (operand[0] == 'C') { // C'EOF'
                for (int i = 2; i < strlen(operand) - 1; i++) {
                    char byte_hex[3];
                    format_hex_2(byte_hex, operand[i]); // Convert char to 2-digit hex
                    strcat(object_code, byte_hex);
                }
            } else if (operand[0] == 'X') { // X'F1'
                for (int i = 2; i < strlen(operand) - 1; i++) {
                    object_code[i-2] = operand[i];
                }
                object_code[strlen(operand) - 3] = '\0';
            }
        } 
        // Note: RESW and RESB generate no object code

        // D. Write to Listing File
        fprintf(f_list, "%s\t%s\t%s\t%s\t%s\n", locctr_str, label, opcode, operand, 
                (strcmp(object_code, "") == 0) ? "-" : object_code);

        // E. Manage Text (T) Record
        if (strcmp(object_code, "") != 0) { // If object code was generated
            
            // If this is the first piece of code for this T-record
            if (t_record_len == 0) {
                t_start_addr = (int)strtol(locctr_str, NULL, 16);
            }

            // Check if it fits (max 60 chars/30 bytes)
            if (t_record_len + strlen(object_code) > 60) {
                // Write current T-record
                fprintf(f_obj, "T^%06X^%02X%s\n", t_start_addr, t_record_len / 2, t_record_buffer);
                // Start new T-record
                t_record_len = strlen(object_code);
                strcpy(t_record_buffer, object_code);
                t_start_addr = (int)strtol(locctr_str, NULL, 16);
            } else {
                // Add to current T-record
                strcat(t_record_buffer, object_code);
                t_record_len += strlen(object_code);
            }

        } else if (strcmp(opcode, "RESW") == 0 || strcmp(opcode, "RESB") == 0) {
            // Break T-record
            if (t_record_len > 0) {
                fprintf(f_obj, "T^%06X^%02X%s\n", t_start_addr, t_record_len / 2, t_record_buffer);
                t_record_len = 0;
                t_record_buffer[0] = '\0';
            }
        }
    } // End of main while loop

    // 6. Write last T-record (if any)
    if (t_record_len > 0) {
        fprintf(f_obj, "T^%06X^%02X%s\n", t_start_addr, t_record_len / 2, t_record_buffer);
    }

    // 7. Write End (E) Record
    fprintf(f_obj, "E^%06X\n", start_addr);

    printf("\nPass 2 complete.\n");
    printf("Object program written to 'object_program.txt'\n");
    printf("Final listing written to 'listing.txt'\n");

    // 8. Close all files
    fclose(f_inter);
    fclose(f_optab);
    fclose(f_symtab);
    fclose(f_list);
    fclose(f_obj);

    return 0;
}

