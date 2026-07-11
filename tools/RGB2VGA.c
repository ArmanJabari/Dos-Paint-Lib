#include <stdio.h>
#include <stdlib.h>

int convert_to_vga(int color8Bit) {
    return color8Bit >> 2; 
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: RGB2VGA <R> <G> <B>\n");
        return 1;
    }

    int r = atoi(argv[1]);
    int g = atoi(argv[2]);
    int b = atoi(argv[3]);

    if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255) {
        printf("Error: RGB values must be between 0 and 255.\n");
        return 1;
    }

    int vgaR = convert_to_vga(r);
    int vgaG = convert_to_vga(g);
    int vgaB = convert_to_vga(b);

    printf("VGA: %d, %d, %d\n", vgaR, vgaG, vgaB);

    return 0;
}
