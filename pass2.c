/*
 * C Program to Simulate Pass 2 of a Two-Pass Assembler
 *
 * This program reads from:
 * 1. intermediate.txt (from Pass 1)
 * 2. symtab.txt       (from Pass 1)
 * 3. length.txt       (from Pass 1)
 * 4. optab.txt        (Opcode Table)
 *
 * It produces:
 * 1. output.txt        (Program Listing with Object Code)
 * 2. object_program.txt (Object Program in H-T-E format)
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Function to search OPTAB for opcode and get machine code
// Returns 1 on success, 0 on failure
int search_optab(const char *opcode, char *machine_code) {
    FILE *f_optab = fopen("optab.txt", "r");
    char op[20], code[10];
    if (f_optab == NULL) return 0;

    while (fscanf(f_optab, "%s %s", op, code) != EOF) {
        if (strcmp(opcode, op) == 0) {
            strcpy(machine_code, code);
            fclose(f_optab);
            return 1;
        }
    }
    fclose(f_optab);
    return 0;
}

// Function to search SYMTAB for label and get address
// Returns 1 on success, 0 on failure
int search_symtab(const char *label, char *address) {
    FILE *f_symtab = fopen("symtab.txt", "r");
    char sym[20], addr[10];
    if (f_symtab == NULL) return 0;

    while (fscanf(f_symtab, "%s %s", sym, addr) != EOF) {
        if (strcmp(label, sym) == 0) {
            strcpy(address, addr);
            fclose(f_symtab);
            return 1;
        }
    }
    fclose(f_symtab);
    return 0;
}

int main() {
    FILE *f_inter, *f_length, *f_output, *f_obj;
    char addr[10], label[20], opcode[20], operand[20];
    char prog_name[20], start_addr[10], prog_len[10];
    char machine_code[10], operand_addr[10], obj_code[20];

    // Buffers for Text Record
    char text_record[70]; // T^001000^1E^...
    char text_buffer[70]; // Holds just the object codes for one record
    int text_len = 0;
    char text_start_addr[10];

    // 1. Open files
    f_inter = fopen("intermediate.txt", "r");
    f_length = fopen("length.txt", "r");
    f_output = fopen("output.txt", "w");
    f_obj = fopen("object_program.txt", "w");

    if (f_inter == NULL || f_length == NULL) {
        printf("Error: Cannot open intermediate.txt or length.txt\n");
        return 1;
    }
    
    // 2. Handle START - Write Header Record
    fscanf(f_inter, "%s %s %s %s", addr, label, opcode, operand);
    fscanf(f_length, "%s", prog_len);

    if (strcmp(opcode, "START") == 0) {
        strcpy(prog_name, label);
        strcpy(start_addr, operand);
        // H^COPY  ^001000^00001B
        fprintf(f_obj, "H^%-6s^%06s^%06s\n", prog_name, start_addr, prog_len);
        
        // Write to listing
        fprintf(f_output, "%s\t%s\t%s\t%s\n", addr, label, opcode, operand);
        
        // Read next line
        fscanf(f_inter, "%s %s %s %s", addr, label, opcode, operand);
    } else {
        // Handle error: no START directive
        return 1; 
    }

    // 3. Initialize first Text Record
    strcpy(text_start_addr, addr); // Start address of first T-record
    text_len = 0;
    memset(text_buffer, 0, sizeof(text_buffer)); // Clear buffers

    // 4. Main Processing Loop
    while (strcmp(opcode, "END") != 0) {
        
        memset(obj_code, 0, sizeof(obj_code)); // Clear obj_code for this line

        if (search_optab(opcode, machine_code)) {
            // A. It's an instruction
            if (strcmp(operand, "-") != 0) {
                if (!search_symtab(operand, operand_addr)) {
                    printf("ERROR: Undefined symbol '%s'\n", operand);
                    strcpy(operand_addr, "0000"); // Use 0000 for error
                }
            } else {
                strcpy(operand_addr, "0000"); // e.g., for RSUB
            }
            // Assume 3-byte instruction: 1 byte opcode + 2 bytes address (SIC)
            // For simplicity, we just use 2 chars for opcode, 4 for address
            snprintf(obj_code, 7, "%s%s", machine_code, operand_addr);
        
        } else if (strcmp(opcode, "WORD") == 0) {
            // B. It's a WORD
            snprintf(obj_code, 7, "%06X", atoi(operand)); // e.g., "1" -> "000001"
        
        } else if (strcmp(opcode, "BYTE") == 0) {
            // B. It's a BYTE
            if (operand[0] == 'C') { // C'EOF'
                for (int i = 2; i < strlen(operand) - 1; i++) {
                    char temp[3];
                    sprintf(temp, "%X", operand[i]); // Get ASCII hex
                    strcat(obj_code, temp);
                }
            } else if (operand[0] == 'X') { // X'F1'
                for (int i = 2; i < strlen(operand) - 1; i++) {
                    strncat(obj_code, &operand[i], 1);
                }
            }
        }
        // C. If RESW or RESB, obj_code remains empty

        // D. Write to Object Program (Text Record)
        
        // Check if T-record needs to be written
        // 1. If new code + current buffer > 60 chars (30 bytes)
        // 2. If this is RESW/RESB (breaks contiguity)
        if ((text_len + strlen(obj_code) > 60) || 
            (strcmp(opcode, "RESW") == 0) || (strcmp(opcode, "RESB") == 0)) {
            
            if (text_len > 0) { // Write the old T-record
                fprintf(f_obj, "T^%06s^%02X%s\n", text_start_addr, text_len / 2, text_buffer);
            }
            // Start a new T-record if this isn't RESW/RESB
            if (strcmp(opcode, "RESW") != 0 && strcmp(opcode, "RESB") != 0) {
                strcpy(text_start_addr, addr);
                text_len = 0;
                memset(text_buffer, 0, sizeof(text_buffer));
            }
        }

        // If object code was generated, add it to the buffer
        if (strlen(obj_code) > 0) {
            strcat(text_buffer, "^");
            strcat(text_buffer, obj_code);
            text_len += strlen(obj_code);
        }

        // E. Write to Listing File
        fprintf(f_output, "%s\t%s\t%s\t%s\t%s\n", addr, label, opcode, operand, obj_code);

        // F. Read Next Line
        fscanf(f_inter, "%s %s %s %s", addr, label, opcode, operand);
    }

    // 5. Handle END
    
    // Write the last Text Record
    if (text_len > 0) {
        fprintf(f_obj, "T^%06s^%02X%s\n", text_start_addr, text_len / 2, text_buffer);
    }
    
    // Write the End Record
    fprintf(f_obj, "E^%06s\n", start_addr);
    
    // Write to listing
    fprintf(f_output, "%s\t%s\t%s\t%s\n", addr, label, opcode, operand);

    // 6. Stop
    fclose(f_inter);
    fclose(f_length);
    fclose(f_output);
    fclose(f_obj);

    printf("\nPass 2 complete.\n");
    printf("Listing file 'output.txt' and 'object_program.txt' created.\n");

    return 0;
}
