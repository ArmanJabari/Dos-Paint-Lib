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

int check_ext(const char* filename, const char* ext) {
    const char* dot = strrchr(filename, '.');
    if (!dot) return 0;
    while (*dot && *ext) {
        char c1 = *dot >= 'A' && *dot <= 'Z' ? *dot + 32 : *dot;
        char c2 = *ext >= 'A' && *ext <= 'Z' ? *ext + 32 : *ext;
        if (c1 != c2) return 0;
        dot++; ext++;
    }
    return (*dot == *ext);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: palconv <file.bmp | file.pcx>\n");
        return 1;
    }

    char *infile = argv[1];
    FILE *f = fopen(infile, "rb");
    if (!f) {
        printf("Error: Could not open %s\n", infile);
        return 1;
    }

    unsigned char pixels[16][16][3];
    int is_pcx = check_ext(infile, ".pcx");
    int is_bmp = check_ext(infile, ".bmp");

    if (is_pcx) {
        unsigned char header[128];
        if (fread(header, 1, 128, f) != 128) {
            printf("Error: Invalid PCX file\n");
            fclose(f);
            return 1;
        }
        
        if (header[0] != 10) {
            printf("Error: Not a valid PCX file\n");
            fclose(f);
            return 1;
        }
        
        int bpp = header[3];
        int nplanes = header[65];
        
        if (bpp == 8 && nplanes == 1) {
            fseek(f, -769, SEEK_END);
            int magic = fgetc(f);
            if (magic != 12) {
                printf("Error: No 256-color palette found in PCX\n");
                fclose(f);
                return 1;
            }
            unsigned char pal[768];
            if (fread(pal, 1, 768, f) != 768) {
                printf("Error: Could not read palette\n");
                fclose(f);
                return 1;
            }
            for (int r = 0; r < 16; r++) {
                for (int c = 0; c < 16; c++) {
                    int idx = (r * 16 + c) * 3;
                    pixels[r][c][0] = pal[idx];
                    pixels[r][c][1] = pal[idx+1];
                    pixels[r][c][2] = pal[idx+2];
                }
            }
        } else if (bpp == 8 && nplanes == 3) {
            int xmin = header[4] | (header[5] << 8);
            int ymin = header[6] | (header[7] << 8);
            int xmax = header[8] | (header[9] << 8);
            int ymax = header[10] | (header[11] << 8);
            int w = xmax - xmin + 1;
            int h = ymax - ymin + 1;
            int bpl = header[66] | (header[67] << 8);

            if (w != 16 || h != 16) {
                printf("Error: Image must be exactly 16x16 pixels.\n");
                fclose(f);
                return 1;
            }

            for (int r = 0; r < 16; r++) {
                for (int p = 0; p < 3; p++) {
                    int x = 0;
                    while (x < bpl) {
                        int c_val = fgetc(f);
                        if (c_val == EOF) break;
                        unsigned char b = (unsigned char)c_val;
                        int count = 1;
                        unsigned char val = b;

                        if ((b & 0xC0) == 0xC0) {
                            count = b & 0x3F;
                            int next_val = fgetc(f);
                            if (next_val == EOF) break;
                            val = (unsigned char)next_val;
                        }

                        for (int i = 0; i < count; i++) {
                            if (x < 16) {
                                pixels[r][x][p] = val;
                            }
                            x++;
                        }
                    }
                }
            }
        } else {
            printf("Error: Only 24-bit or 8-bit PCX files are supported.\n");
            fclose(f);
            return 1;
        }
    } else if (is_bmp) {
        unsigned char header[54];
        if (fread(header, 1, 54, f) != 54 || header[0] != 'B' || header[1] != 'M') {
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

        fseek(f, offset, SEEK_SET);

        for (int r = 0; r < 16; r++) {
            int row = is_bottom_up ? (15 - r) : r;
            for (int c = 0; c < 16; c++) {
                unsigned char bgr[3];
                if (fread(bgr, 1, 3, f) != 3) break;
                pixels[row][c][0] = bgr[2];
                pixels[row][c][1] = bgr[1];
                pixels[row][c][2] = bgr[0];
            }
        }
    } else {
        printf("Error: Unsupported file format. Use .bmp or .pcx\n");
        fclose(f);
        return 1;
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
