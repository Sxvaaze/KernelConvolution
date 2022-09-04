#include <stdio.h>
#include <math.h>
#include "readwritebmp.c"

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Syntax: ./a.out dimension mask_file bmp_file");
        return -1;
    }

    int mask_dimension = atoi(argv[1]);
    const char* mask_file = argv[2];
    const char* bmp_file = argv[3];

    double* kernel = (double *)malloc(mask_dimension * mask_dimension * sizeof(double));
    FILE* fp = fopen(mask_file, "r");
    unsigned int i,j;
    for (i = 0; i < mask_dimension; i++) {
        for (j = 0; j < mask_dimension; j++) {
            fscanf(fp, "%lf", &kernel[i * mask_dimension + j]);
        }
        fscanf(fp, "\n");
    }

    BMPImage_t* bmp = ReadBMP(bmp_file);

    int upper_half = floor(mask_dimension / 2);
    int lower_half = ceil(mask_dimension / 2);

    int32_t WIDTH = bmp->header.width_px;
    int32_t HEIGHT = bmp->header.height_px;

    RGB_t* rgb_values = malloc(HEIGHT * WIDTH * sizeof(RGB_t));

    // Άθροισματα γινομένων της μορφής (τιμή pixel) * kernel[pos] 
    RGB_t colour_pix;
    unsigned int w, h = 0;
    while (h < HEIGHT) {
        w = 0;
        while (w < WIDTH) {
            int colours[3] = {0, 0, 0}; // (R,G,B)
            for (int z = -lower_half, counter_a = 0; z <= upper_half && counter_a < mask_dimension; z++, counter_a++) {
                for (int q = -lower_half, counter_b = 0; q <= upper_half && counter_b < mask_dimension; q++, counter_b++) {
                    if (w + q < 0 || h + z < 0 || w + q >= WIDTH || h + z >= HEIGHT) continue;
                    colour_pix = GetPixel(bmp, w + q, h + z);
                    colours[0] += colour_pix.r * kernel[3*counter_a + counter_b];
                    colours[1] += colour_pix.g * kernel[3*counter_a + counter_b];
                    colours[2] += colour_pix.b * kernel[3*counter_a + counter_b];
                }
            }

            RGB_t colour;
            colour.r = colours[0];
            colour.g = colours[1];
            colour.b = colours[2];
            rgb_values[HEIGHT * w + h] = colour;
            w++;
        }
        h++;
    }

    for (h = 0; h < HEIGHT; h++) {
        for (w = 0; w < WIDTH; w++) {
            SetPixel(bmp, w, h, rgb_values[HEIGHT * w + h]);
        }
    }

    ApouhkeyshBMP(bmp, mask_file);
    return 0;
}