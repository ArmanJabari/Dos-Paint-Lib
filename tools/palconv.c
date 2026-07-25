#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <direct.h>
#define MAKE_DIR(path) _mkdir(path)
#else
#include <sys/stat.h>
#include <sys/types.h>
#define MAKE_DIR(path) mkdir(path, 0777)
#endif

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: palconv <file.bmp>\n");
        return 1;
    }

    char *infile = argv[1];
    FILE *f = fopen(infile, "rb");
    if (!f) {
        printf("Error: Could not open %s\n", infile);
        return 1;
    }

    unsigned char header[54];
    if (fread(header, 1, 54, f) != 54) {
        printf("Error: Invalid BMP file\n");
        fclose(f);
        return 1;
    }

    if (header[0] != 'B' || header[1] != 'M') {
        printf("Error: Not a valid BMP file\n");
        fclose(f);
        return 1;
    }

    int offset = header[10] | (header[11] << 8) | (header[12] << 16) | (header[13] << 24);
    int width  = header[18] | (header[19] << 8) | (header[20] << 16) | (header[21] << 24);
    int height = header[22] | (header[23] << 8) | (header[24] << 16) | (header[25] << 24);
    int bpp    = header[28] | (header[29] << 8);

    int is_bottom_up = 1;
    if (height < 0) {
        is_bottom_up = 0;
        height = -height;
    }

    if (width != 16 || height != 16) {
        printf("Error: Image must be exactly 16x16 pixels.\n");
        fclose(f);
        return 1;
    }

    if (bpp != 24) {
        printf("Error: Only 24-bit BMP files are supported.\n");
        fclose(f);
        return 1;
    }

    unsigned char pixels[16][16][3];

    fseek(f, offset, SEEK_SET);

    for (int r = 0; r < 16; r++) {
        int row = is_bottom_up ? (15 - r) : r;
        for (int c = 0; c < 16; c++) {
            unsigned char bgr[3];
            fread(bgr, 1, 3, f);
            pixels[row][c][0] = bgr[2];
            pixels[row][c][1] = bgr[1];
            pixels[row][c][2] = bgr[0];
        }
    }
    fclose(f);

    char outfile[256];
    char label[256];
    strncpy(outfile, infile, sizeof(outfile) - 1);
    outfile[sizeof(outfile) - 1] = '\0';

    char *dot = strrchr(outfile, '.');
    if (dot) *dot = '\0';

    char *slash1 = strrchr(outfile, '/');
    char *slash2 = strrchr(outfile, '\\');
    char *base = outfile;
    if (slash1 > base) base = slash1 + 1;
    if (slash2 > base) base = slash2 + 1;

    strncpy(label, base, sizeof(label) - 1);
    label[sizeof(label) - 1] = '\0';

    MAKE_DIR("../assets");
    MAKE_DIR("../assets/palattes");

    char outpath[512];
    snprintf(outpath, sizeof(outpath), "../assets/palattes/%s.inc", base);

    FILE *out = fopen(outpath, "w");
    if (!out) {
        printf("Error: Could not create output file %s\n", outpath);
        return 1;
    }

    if (label[0] >= '0' && label[0] <= '9') {
        printf("Notice: Image name '%s' starts with a number. Appending '_' to assembly label.\n", label);
        fprintf(out, "_%s:\n", label);
    } else {
        fprintf(out, "%s:\n", label);
    }

    for (int r = 0; r < 16; r++) {
        for (int c = 0; c < 16; c++) {
            int r_val = pixels[r][c][0] >> 2;
            int g_val = pixels[r][c][1] >> 2;
            int b_val = pixels[r][c][2] >> 2;
            fprintf(out, "    db %d, %d, %d\n", r_val, g_val, b_val);
        }
    }

    fclose(out);
    printf("Success: %s -> %s\n", infile, outpath);

    return 0;
}