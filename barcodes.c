#include <stdio.h>
#include "string.h"
#include "barcodes.h"


int EAN8_algConvert(int alg, POS_CODE_t pos, char *buf){
    if(alg < 0 || alg > 9){
        return -1;
    }
    switch(pos){
        case L_CODE:
            memcpy(buf, l_codes[alg], 8);
            break;
        case R_CODE:
            memcpy(buf, r_codes[alg], 8);
            break;
        default:
            return -1;
    }

    return 0;
}

 char EAN8_algConvert_reverse(char* alg, POS_CODE_t pos) {
    switch (pos) {
        case L_CODE:
            for (int i = 0; i < 10; i++) {  // Loop through all 10 codes in l_codes
                if (strcmp(alg, l_codes[i]) == 0) {
                    return '0' + i;  // Return the character corresponding to the index
                }
            }
            break;  // Prevent fall-through to next case
        
        case R_CODE:
            for (int i = 0; i < 10; i++) {  // Loop through all 10 codes in r_codes
                if (strcmp(alg, r_codes[i]) == 0) {
                    return '0' + i;  // Return the character corresponding to the index
                }
            }
            break;  // Prevent fall-through to next case
        
        default:
            return -1;  // Invalid position
    }

    return -1;  // Return -1 if no match is found
}

void EAN8_read_barcode(char *name) {
    int line_size;
    int lines;
    char real_barcode[67];
    char result[8] = "";
    int zero_lines = 0;  // Initialize zero_lines to 0
    char line_temp[1024];  // Buffer to hold each line (assuming each line is not larger than 1024 characters)
    FILE *file = fopen(name, "r");

    if (!file) {
        printf("Error opening file!\n");
        return;
    }

    // Read first line that contains P1 (not used further)
    fgets(line_temp, sizeof(line_temp), file);

    // Read the second line that contains the size of the lines and columns
    fgets(line_temp, sizeof(line_temp), file);
    sscanf(line_temp, "%d %d", &line_size, &lines);

    // Allocate memory for the barcode 2D array
    char **barcode = malloc(lines * sizeof(char *));
    if (!barcode) {
        printf("Memory allocation failed!\n");
        fclose(file);
        return;
    }

    for (int i = 0; i < lines; i++) {
        barcode[i] = malloc(line_size * sizeof(char));  
    }

    // Read the barcode data into the 2D array
    for (int i = 0; i < lines; i++) {
        fgets(line_temp, sizeof(line_temp), file);  
        for (int j = 0; j < line_size; j++) {
            barcode[i][j] = line_temp[j];  
        }
    }

    // Count the number of zero rows before the actual barcode starts
    for (int line = 0; line < lines; line++) {
        int soma_pixels = 0;
        for (int column = 0; column < line_size; column++) {
            if (barcode[line][column] == '1') {  
                soma_pixels++;
            }
        }

        if (soma_pixels > 0) {
            break;  
        }

        zero_lines++;
    }

    // Calculate the skip value based on pixel density
    int skip = (line_size - 2 * zero_lines) / 67;
    int z = 0;  
    int l = lines/2;
    // Process the barcode from the row after the zero lines
    for (int j = zero_lines; j < line_size - zero_lines; j += skip) {
        real_barcode[z] = barcode[l][j];  
        z++;
    }

    real_barcode[z] = '\0'; 

    for (int i = 3; i < 31; i += 7) {
        char chunk[8];  
        int j;
        for (j = 0; j < 7 && i + j < 67; j++) {
            chunk[j] = real_barcode[i + j];
        }
        chunk[j] = '\0'; 
        char digit = EAN8_algConvert_reverse(chunk,L_CODE);
        strncat(result, &digit,1);
    }
    for (int i = 36; i < 64; i += 7) {
        char chunk[8];  
        int j;
        for (j = 0; j < 7 && i + j < 67; j++) {
            chunk[j] = real_barcode[i + j];
        }
        chunk[j] = '\0'; 
        char digit = EAN8_algConvert_reverse(chunk,R_CODE);
        strncat(result, &digit,1);
    }
    printf("%s",result);

    // Free allocated memory for the barcode 2D array
    for (int i = 0; i < lines; i++) {
        free(barcode[i]);
    }
    free(barcode);

    fclose(file);  // Close the file
}

void check_valid(int num) {
    int checker[7];  // Array to store the first 7 digits
    int last_digit;  // Variable to store the last digit
    
    last_digit = num % 10;
    
    num = num / 10;
    

    for (int i = 6; i >= 0; i--) {  
        checker[i] = num % 10;
        num = num / 10;
    }
    

    int identifier = 0;
    for (int i = 0; i < 7; i += 2) {  
        identifier += 3*checker[i];
    }
    
    identifier += checker[1] + checker[3] + checker[5];
    int count = 0;
    while (count < identifier){
        count += 10;
    }
    identifier = count - identifier;
    if(identifier != last_digit){
        printf("verifier digit is incorrect");
        abort();
    }
}
void EAN8_num_converter(int numero, char *buf, size_t buf_size){
    memset(buf, 0, buf_size);

    while(numero > 99999999 || numero < 9999999){
        printf("invalid number(has to have 8 digits)");
        scanf("%d", &numero);
    }

    //checar se e valido
    check_valid(numero);
    int l_code[4];
    int r_code[4];

    // splits the number in left and right
    for(int i = 3; i >= 0; i--){
        r_code[i] = numero % 10;
        numero = numero / 10;
    }

    for(int i = 3; i >= 0; i--){
        l_code[i] = numero % 10;
        numero = numero / 10;
    }


    // transforms a digit from l_code and r_code 
    strcat(buf, "101");
    for(int i = 0; i < 4; i ++){
        char alg_str[8] = {0};
        EAN8_algConvert(l_code[i], L_CODE, alg_str);
        strncat(buf, alg_str, 8);
    }
    strcat(buf, "01010");
    for(int i = 0; i < 4; i ++){
        char alg_str[8] = {0};
        EAN8_algConvert(r_code[i], R_CODE, alg_str);
        strncat(buf, alg_str, 8);
    }
    strcat(buf, "101");
}





  
int file_exists(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return 1;  // File exists
    }
    return 0;  // File does not exist
}

void EAN8_write_barcode(FILE *arquivo, char *barcode_data, int width, int height, int pixels_per_bar, int pixels_side) {
    // Create the top space
    for (int i = 0; i < pixels_side; i++) {
        for (int j = 0; j < (width * pixels_per_bar) + 2*pixels_side; j++) {
            fputc('0', arquivo);  
        }
        fputc('\n', arquivo);
    }

    // Write barcode data
    for (int i = 0; i < height; i++) {
        for(int z = 0;z < pixels_side;z++){
                fputc('0',arquivo);
         }
        for (int j = 0; j < width; j++) {
            char current_pixel = barcode_data[j];
            // Write pixels per bar (width of the bars)
            for (int k = 0; k < pixels_per_bar; k++) {
                fputc(current_pixel, arquivo);
            }
        }
        for(int z = 0;z < pixels_side;z++){
            fputc('0',arquivo);
        }
        fputc('\n', arquivo);  // Newline after each row of pixels
    }

    // Add bottom space
    for (int i = 0; i < pixels_side; i++) {
        for (int j = 0; j < (width * pixels_per_bar) + 2*pixels_side; j++) {
            fputc('0', arquivo);  
        }
        fputc('\n', arquivo);
    }
}