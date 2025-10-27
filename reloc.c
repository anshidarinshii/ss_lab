#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void convert(char hex_mask[], char bit_mask[]);
void display_file_content(const char *filename);

int main(void) {
    char add_str[7], length_str[7], input[10], bitmask_hex[13], pn[7];
    char bitmask_bin[49];
    int start_addr, text_addr, text_len, i, opcode, addr, actual_addr;
    FILE *fp1, *fp2;

    printf("Enter the actual starting address: ");
    scanf("%x", &start_addr);

    fp1 = fopen("RLIN.txt", "r");
    if (fp1 == NULL) {
        printf("Error: Cannot open RLIN.txt\n");
        return 1;
    }

    fp2 = fopen("RLOUT.txt", "w");
    if (fp2 == NULL) {
        printf("Error: Cannot open RLOUT.txt\n");
        fclose(fp1);
        return 1;
    }

    fscanf(fp1, "%s", input);

    fprintf(fp2, "----------------------------\n");
    fprintf(fp2, " ADDRESS   CONTENT\n");
    fprintf(fp2, "----------------------------\n");

    while (strcmp(input, "E") != 0) {
        if (strcmp(input, "H") == 0) {
            fscanf(fp1, "%s %s %s", pn, add_str, length_str);
            fscanf(fp1, "%s", input);
        }

        if (strcmp(input, "T") == 0) {
            fscanf(fp1, "%x", &text_addr);
            fscanf(fp1, "%x", &text_len);
            fscanf(fp1, "%s", bitmask_hex);

            text_addr += start_addr;

            convert(bitmask_hex, bitmask_bin);

            int num_instructions = text_len / 3;

            for (i = 0; i < num_instructions; i++) {
                fscanf(fp1, "%x %x", &opcode, &addr);

                if (bitmask_bin[i] == '1') {
                    actual_addr = addr + start_addr;
                } else {
                    actual_addr = addr;
                }

                fprintf(fp2, " %04X\t   %02X%04X\n", text_addr, opcode, actual_addr);

                text_addr += 3;
            }
            fscanf(fp1, "%s", input);
        }
    }

    fprintf(fp2, "----------------------------\n");

    fclose(fp2);
    fclose(fp1);

    printf("\n Relocating loader finished.\n");

    printf("\n\n--- Content of RLIN.txt ---\n");
    display_file_content("RLIN.txt");

    printf("\n\n--- Content of RLOUT.txt ---\n");
    display_file_content("RLOUT.txt");
    printf("\n");

    return 0;
}

void convert(char hex_mask[], char bit_mask[]) {
    int i;
    strcpy(bit_mask, "");

    for (i = 0; i < strlen(hex_mask); i++) {
        switch (hex_mask[i]) {
            case '0': strcat(bit_mask, "0000"); break;
            case '1': strcat(bit_mask, "0001"); break;
            case '2': strcat(bit_mask, "0010"); break;
            case '3': strcat(bit_mask, "0011"); break;
            case '4': strcat(bit_mask, "0100"); break;
            case '5': strcat(bit_mask, "0101"); break;
            case '6': strcat(bit_mask, "0110"); break;
            case '7': strcat(bit_mask, "0111"); break;
            case '8': strcat(bit_mask, "1000"); break;
            case '9': strcat(bit_mask, "1001"); break;
            case 'A': case 'a': strcat(bit_mask, "1010"); break;
            case 'B': case 'b': strcat(bit_mask, "1011"); break;
            case 'C': case 'c': strcat(bit_mask, "1100"); break;
            case 'D': case 'd': strcat(bit_mask, "1101"); break;
            case 'E': case 'e': strcat(bit_mask, "1110"); break;
            case 'F': case 'f': strcat(bit_mask, "1111"); break;
        }
    }
}

void display_file_content(const char *filename) {
    char line_buffer[256];
    FILE *file_ptr = fopen(filename, "r");

    if (file_ptr == NULL) {
        printf("Error reading %s back.\n", filename);
        return;
    }

    while (fgets(line_buffer, sizeof(line_buffer), file_ptr) != NULL) {
        printf("%s", line_buffer);
    }
    
    fclose(file_ptr);
}
