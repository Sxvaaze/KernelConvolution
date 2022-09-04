
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp.h"

void ApouhkeyshBMP(BMPImage_t* bmp, const char* filename) {
    // Στο αρχείο μορφής <name>.bmp κάνε διαχωρισμό ώστε να κρατήσεις μόνο το <name> 
    char* maskfile_name_copy = strdup(filename);
    char* token = strtok(maskfile_name_copy, ".");

    // Μορφή "output_serial.<mask1 χωρίς το extension του αρχείου>"
    int length = 0;
    char buffer[4096];
    length += sprintf(buffer, "output_serial.");
    length += sprintf(buffer+length, token);

    // Αποθήκευση bitmap img
    SaveBMP(bmp, strcat(buffer, ".bmp"));
    DestroyBMP(bmp);
}