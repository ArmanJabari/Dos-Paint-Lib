#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>

static uint8_t convert_to_vga(uint8_t color8Bit) {
    return color8Bit >> 2; 
}

static int parse_color_channel(const char *str, uint8_t *out_val) {
    char *endptr;
    errno = 0; 
    
    long val = strtol(str, &endptr, 10);

    if (errno != 0 || *endptr != '\0' || str == endptr) {
        return 0; 
    }
    
    if (val < 0 || val > 255) {
        return 0; 
    }

    *out_val = (uint8_t)val;
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <R> <G> <B>\n", argv[0]);
        fprintf(stderr, "Converts 8-bit RGB channels to 6-bit VGA DAC values.\n");
        return EXIT_FAILURE;
    }

    uint8_t r, g, b;

    if (!parse_color_channel(argv[1], &r) || 
        !parse_color_channel(argv[2], &g) || 
        !parse_color_channel(argv[3], &b)) {
        fprintf(stderr, "Error: RGB values must be valid integers between 0 and 255.\n");
        return EXIT_FAILURE;
    }

    uint8_t vga_r = convert_to_vga(r);
    uint8_t vga_g = convert_to_vga(g);
    uint8_t vga_b = convert_to_vga(b);

    printf("%u, %u, %u\n", vga_r, vga_g, vga_b);

    return EXIT_SUCCESS;
}
