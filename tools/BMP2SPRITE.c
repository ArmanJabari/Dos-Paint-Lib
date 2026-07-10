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

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <image.bmp>\n", argv[0]);
        return 1;
    }

    FILE *fIn = fopen(argv[1], "rb");
    if (!fIn) {
        printf("Error: Could not open %s\n", argv[1]);
        return 1;
    }

    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;
    
    fread(&bfh, sizeof(BITMAPFILEHEADER), 1, fIn);
    fread(&bih, sizeof(BITMAPINFOHEADER), 1, fIn);

    if (bfh.bfType != 0x4D42) {
        printf("Error: Not a valid BMP file.\n");
        fclose(fIn);
        return 1;
    }

    if (bih.biBitCount != 8) {
        printf("Error: BMP must be 8-bit (256 colors). Current depth: %d-bit\n", bih.biBitCount);
        fclose(fIn);
        return 1;
    }

    char baseName[256];
    extract_name(argv[1], baseName);
    
    MKDIR("../sprite");

    char outName[256];
    sprintf(outName, "../sprite/%s.inc", baseName);

    FILE *fOut = fopen(outName, "w");
    if (!fOut) {
        printf("Error: Could not create %s\n", outName);
        fclose(fIn);
        return 1;
    }

    int width = bih.biWidth;
    int height = bih.biHeight;
    
    int isBottomUp = (height > 0);
    if (!isBottomUp) {
        height = -height;
    }

    int rowPadded = (width + 3) & (~3);
    
    unsigned char *pixels = (unsigned char *)malloc(rowPadded * height);
    
    if (pixels == NULL) {
        printf("Error: Memory allocation failed. Image might be too large or corrupted.\n");
        fclose(fOut);
        fclose(fIn);
        return 1;
    }
    
    fseek(fIn, bfh.bfOffBits, SEEK_SET);
    fread(pixels, 1, rowPadded * height, fIn);
    fclose(fIn);

    fprintf(fOut, "; Sprite dimensions: %dx%d\n", width, height);
    
    if (baseName[0] >= '0' && baseName[0] <= '9') {
        printf("Notice: Image name '%s' starts with a number. Appending '_' to assembly label.\n", baseName);
        fprintf(fOut, "_%s:\n", baseName);
    } else {
        fprintf(fOut, "%s:\n", baseName);
    }

    for (int y = 0; y < height; y++) {
        int actualY = isBottomUp ? (height - 1 - y) : y;
        
        fprintf(fOut, "    db ");
        
        for (int x = 0; x < width; x++) {
            unsigned char p = pixels[actualY * rowPadded + x];
            
            fprintf(fOut, "%3d", p);
            
            if (x < width - 1) {
                fprintf(fOut, ", ");
            }
        }
        fprintf(fOut, "\n");
    }

    free(pixels);
    fclose(fOut);

    return 0;
}
