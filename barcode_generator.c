#include <stdio.h>
#include "string.h"
#include "barcodes.c"
#include <stdlib.h>



int main(){
    printf("say the barcode you wanna make:\n");
    int var;
    scanf("%d", &var);
    char num_code_full[67] = {0}; 
    EAN8_num_converter(var, num_code_full, 67);
    int width = strlen(num_code_full);  // The width is determined by the length of barcode_data

    // Default barcode parameters
    int pixels_side = 4;
    int height = 20;              
    int pixels_per_bar = 2;                 

    // Optionally modify barcode parameters
    char YN;
    printf("write Y if you want to change the configurations of the file, or N if you dont: ");
    scanf(" %c", &YN);

    if (YN == 'Y' || YN == 'y') {
        printf("side spacing:  ");
        scanf("%d", &pixels_side);
        printf("height: ");
        scanf("%d", &height);
        printf("pixels_per_bar: ");
        scanf("%d", &pixels_per_bar);
    }

    // Get the filename from the user
    char name[100];
    printf("file name: ");
    scanf(" %99[^\n]", name);
    strcat(name, ".pbm");

    // Check if the file already exists
    if (file_exists(name)) {
        char choice;
        printf("this file %s already exists, do you want to rewrite it? (Y/N): ", name);
        scanf(" %c", &choice);
        if (choice != 'Y' && choice != 'y') {
            printf("file already exists.\n");
            return 0;
        }
    }

    // Open the PBM file for writing
    FILE *arquivo = fopen(name, "w");
    if (arquivo == NULL) {
        printf("Error openning file!\n");
        return 1;
    }

    // Adjusting the height and width based on the padding and pixel width
    int total_height = height + 2 * pixels_side; 
    int total_width = (width * pixels_per_bar) + 2*pixels_side;
    // Write PBM header
    fprintf(arquivo, "P1\n");
    fprintf(arquivo, "%d %d\n", total_width, total_height); 

    // Write the barcode pattern
    EAN8_write_barcode(arquivo, num_code_full, width, height, pixels_per_bar, pixels_side);

    fclose(arquivo);

    printf("file created succesfully.\n");
    return 0;
}


