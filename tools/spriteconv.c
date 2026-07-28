#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <direct.h>
#define MKDIR(a) _mkdir(a)
#else
#include <sys/stat.h>
#define MKDIR(a) mkdir(a, 0777)
#endif

#pragma pack(push, 1)

typedef struct {
    unsigned short bfType;
    unsigned int bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned int bfOffBits;
} BITMAPFILEHEADER;

typedef struct {
    unsigned int biSize;
    int biWidth;
    int biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned int biCompression;
    unsigned int biSizeImage;
    int biXPelsPerMeter;
    int biYPelsPerMeter;
    unsigned int biClrUsed;
    unsigned int biClrImportant;
} BITMAPINFOHEADER;

typedef struct {
    unsigned char Manufacturer;
    unsigned char Version;
    unsigned char Encoding;
    unsigned char BitsPerPixel;
    unsigned short XMin;
    unsigned short YMin;
    unsigned short XMax;
    unsigned short YMax;
    unsigned short HDpi;
    unsigned short VDpi;
    unsigned char Colormap[48];
    unsigned char Reserved;
    unsigned char NPlanes;
    unsigned short BytesPerLine;
    unsigned short PaletteInfo;
    unsigned short HscreenSize;
    unsigned short VscreenSize;
    unsigned char Filler[54];
} PCXHEADER;

#pragma pack(pop)

void extract_name(const char *filepath, char *name_out) {
    const char *slash = strrchr(filepath, '/');
    const char *backslash = strrchr(filepath, '\\');
    const char *start = (slash > backslash ? slash : backslash);
    start = start ? start + 1 : filepath;
    strcpy(name_out, start);
    char *dot = strrchr(name_out, '.');
    if (dot) {
        *dot = '\0';
    }
}

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
        printf("Usage: %s <image.bmp | image.pcx>\n", argv[0]);
        return 1;
    }

    FILE *fIn = fopen(argv[1], "rb");
    if (!fIn) {
        printf("Error: Could not open %s\n", argv[1]);
        return 1;
    }

    int width = 0, height = 0;
    unsigned char *pixels = NULL;

    if (check_ext(argv[1], ".pcx")) {
        PCXHEADER pcx;
        fread(&pcx, sizeof(PCXHEADER), 1, fIn);
        
        if (pcx.Manufacturer != 10 || pcx.Encoding != 1 || pcx.BitsPerPixel != 8 || pcx.NPlanes != 1) {
            printf("Error: Unsupported PCX format. Must be 8-bit 256 colors.\n");
            fclose(fIn);
            return 1;
        }
        
        width = pcx.XMax - pcx.XMin + 1;
        height = pcx.YMax - pcx.YMin + 1;
        pixels = (unsigned char *)malloc(width * height);
        
        for (int y = 0; y < height; y++) {
            int x = 0;
            while (x < pcx.BytesPerLine) {
                int c = fgetc(fIn);
                if (c == EOF) break;
                unsigned char b = (unsigned char)c;
                int count = 1;
                unsigned char val = b;
                
                if ((b & 0xC0) == 0xC0) {
                    count = b & 0x3F;
                    c = fgetc(fIn);
                    if (c == EOF) break;
                    val = (unsigned char)c;
                }
                
                for (int i = 0; i < count; i++) {
                    if (x < width) {
                        pixels[y * width + x] = val;
                    }
                    x++;
                }
            }
        }
        fclose(fIn);
        
    } else if (check_ext(argv[1], ".bmp")) {
        BITMAPFILEHEADER bfh;
        BITMAPINFOHEADER bih;
        
        fread(&bfh, sizeof(BITMAPFILEHEADER), 1, fIn);
        fread(&bih, sizeof(BITMAPINFOHEADER), 1, fIn);
        
        if (bfh.bfType != 0x4D42 || bih.biBitCount != 8) {
            printf("Error: Not a valid 8-bit BMP file.\n");
            fclose(fIn);
            return 1;
        }
        
        width = bih.biWidth;
        height = bih.biHeight;
        
        int isBottomUp = (height > 0);
        if (!isBottomUp) height = -height;
        
        int rowPadded = (width + 3) & (~3);
        unsigned char *raw = (unsigned char *)malloc(rowPadded * height);
        
        fseek(fIn, bfh.bfOffBits, SEEK_SET);
        fread(raw, 1, rowPadded * height, fIn);
        fclose(fIn);
        
        pixels = (unsigned char *)malloc(width * height);
        
        for (int y = 0; y < height; y++) {
            int actualY = isBottomUp ? (height - 1 - y) : y;
            for (int x = 0; x < width; x++) {
                pixels[y * width + x] = raw[actualY * rowPadded + x];
            }
        }
        free(raw);
        
    } else {
        printf("Error: Unsupported file format. Use .bmp or .pcx\n");
        fclose(fIn);
        return 1;
    }

    if (!pixels) {
        printf("Error: Memory allocation failed.\n");
        return 1;
    }

    char baseName[256];
    extract_name(argv[1], baseName);
    
    MKDIR("../assets");
    MKDIR("../assets/sprites");

    char outName[512];
    sprintf(outName, "../assets/sprites/%s.inc", baseName);

    FILE *fOut = fopen(outName, "w");
    if (!fOut) {
        printf("Error: Could not create %s\n", outName);
        free(pixels);
        return 1;
    }

    if (baseName[0] >= '0' && baseName[0] <= '9') {
        printf("Notice: Image name '%s' starts with a number. Appending '_' to assembly label.\n", baseName);
        fprintf(fOut, "_%s:\n", baseName);
    } else {
        fprintf(fOut, "%s:\n", baseName);
    }

    for (int y = 0; y < height; y++) {
        fprintf(fOut, "    db ");
        for (int x = 0; x < width; x++) {
            fprintf(fOut, "%3d", pixels[y * width + x]);
            if (x < width - 1) {
                fprintf(fOut, ", ");
            }
        }
        fprintf(fOut, "\n");
    }

    free(pixels);
    fclose(fOut);

    printf("Success: %s -> %s\n", argv[1], outName);
    return 0;
}
