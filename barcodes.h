#include <stdio.h>
#ifndef BARCODES_H
#define BARCODES_H

typedef enum{
    L_CODE = 0,
    R_CODE = 1,
}POS_CODE_t;

char *r_codes[] = {
    "1110010",  // R Code for 0
    "1100110",  // R Code for 1
    "1101100",  // R Code for 2
    "1000010",  // R Code for 3
    "1011100",  // R Code for 4
    "1001110",  // R Code for 5
    "1010000",  // R Code for 6
    "1000100",  // R Code for 7
    "1001000",  // R Code for 8
    "1110100"   // R Code for 9
};

char *l_codes[] = {
    "0001101",  // L Code for 0
    "0011001",  // L Code for 1
    "0010011",  // L Code for 2
    "0111101",  // L Code for 3
    "0100011",  // L Code for 4
    "0110001",  // L Code for 5
    "0101111",  // L Code for 6
    "0111011",  // L Code for 7
    "0110111",  // L Code for 8
    "0001011"   // L Code for 9
};

char EAN8_algConvert_reverse(char* alg, POS_CODE_t pos);

void EAN8_read_barcode(char *name);

int file_exists(const char *filename);//verifies if the file exists 

void EAN8_write_barcode(FILE *arquivo, char *barcode_data, int width, int height, int pixels_per_bar, int pixels_side);//writes the barcode in the file

void EAN8_num_converter(int numero, char *buf, size_t buf_size);//converts to EAN8 the code

void check_valid(int num);//checks if the number is valid


#endif