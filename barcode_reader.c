#include <stdio.h>
#include "barcodes.c"
#include <string.h>
#include <stdlib.h>


int main(){
    char name[20];
    printf("say the file barcode you wanna read\n");
    scanf("%s",name);
    strcat(name,".pbm");
    if (file_exists(name)) {
        EAN8_read_barcode(name);
    }else{
        printf("file with that name doesnt exist");
        return 0;
    }
    EAN8_read_barcode(name);
}