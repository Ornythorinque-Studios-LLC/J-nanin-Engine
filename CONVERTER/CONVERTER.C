#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define MAX_WIDTH 128
#define MAX_HEIGHT 128
#define TILE_MAX (MAX_WIDTH * MAX_HEIGHT)

uint8_t tileBuffer[TILE_MAX];
int tileSizeX = 0, tileSizeY = 0;

typedef struct {
    uint8_t r, g, b;
} color;

color bmp_palette[256];

const unsigned char vga_palette[256][3] = {
    {  0,  0,  0}, {  0,  0,170}, {  0,170,  0}, {  0,170,170}, {170,  0,  0}, {170,  0,170}, {170, 85,  0}, {170,170,170},
    { 85, 85, 85}, { 85, 85,255}, { 85,255, 85}, { 85,255,255}, {255, 85, 85}, {255, 85,255}, {255,255, 85}, {255,255,255},
    {  0,  0,  0}, { 20, 20, 20}, { 32, 32, 32}, { 44, 44, 44}, { 56, 56, 56}, { 69, 69, 69}, { 81, 81, 81}, { 97, 97, 97},
    {113,113,113}, {130,130,130}, {146,146,146}, {162,162,162}, {182,182,182}, {203,203,203}, {227,227,227}, {255,255,255},
    {  0,  0,255}, { 65,  0,255}, {125,  0,255}, {190,  0,255}, {255,  0,255}, {255,  0,190}, {255,  0,125}, {255,  0, 65},
    {255,  0,  0}, {255, 65,  0}, {255,125,  0}, {255,190,  0}, {255,255,  0}, {190,255,  0}, {125,255,  0}, { 65,255,  0},
    {  0,255,  0}, {  0,255, 65}, {  0,255,125}, {  0,255,190}, {  0,255,255}, {  0,190,255}, {  0,125,255}, {  0, 65,255},
    {125,125,255}, {158,125,255}, {190,125,255}, {223,125,255}, {255,125,255}, {255,125,223}, {255,125,190}, {255,125,158},
    {255,125,125}, {255,158,125}, {255,190,125}, {255,223,125}, {255,255,125}, {223,255,125}, {190,255,125}, {158,255,125},
    {125,255,125}, {125,255,158}, {125,255,190}, {125,255,223}, {125,255,255}, {125,223,255}, {125,190,255}, {125,158,255},
    {182,182,255}, {199,182,255}, {219,182,255}, {235,182,255}, {255,182,255}, {255,182,235}, {255,182,219}, {255,182,199},
    {255,182,182}, {255,199,182}, {255,219,182}, {255,235,182}, {255,255,182}, {235,255,182}, {219,255,182}, {199,255,182},
    {182,255,182}, {182,255,199}, {182,255,219}, {182,255,235}, {182,255,255}, {182,235,255}, {182,219,255}, {182,199,255},
    {  0,  0,113}, { 28,  0,113}, { 56,  0,113}, { 85,  0,113}, {113,  0,113}, {113,  0, 85}, {113,  0, 56}, {113,  0, 28},
    {113,  0,  0}, {113, 28,  0}, {113, 56,  0}, {113, 85,  0}, {113,113,  0}, { 85,113,  0}, { 56,113,  0}, { 28,113,  0},
    {  0,113,  0}, {  0,113, 28}, {  0,113, 56}, {  0,113, 85}, {  0,113,113}, {  0, 85,113}, {  0, 56,113}, {  0, 28,113},
    { 56, 56,113}, { 69, 56,113}, { 85, 56,113}, { 97, 56,113}, {113, 56,113}, {113, 56, 97}, {113, 56, 85}, {113, 56, 69},
    {113, 56, 56}, {113, 69, 56}, {113, 85, 56}, {113, 97, 56}, {113,113, 56}, { 97,113, 56}, { 85,113, 56}, { 69,113, 56},
    { 56,113, 56}, { 56,113, 69}, { 56,113, 85}, { 56,113, 97}, { 56,113,113}, { 56, 97,113}, { 56, 85,113}, { 56, 69,113},
    { 81, 81,113}, { 89, 81,113}, { 97, 81,113}, {105, 81,113}, {113, 81,113}, {113, 81,105}, {113, 81, 97}, {113, 81, 89},
    {113, 81, 81}, {113, 89, 81}, {113, 97, 81}, {113,105, 81}, {113,113, 81}, {105,113, 81}, { 97,113, 81}, { 89,113, 81},
    { 81,113, 81}, { 81,113, 89}, { 81,113, 97}, { 81,113,105}, { 81,113,113}, { 81,105,113}, { 81, 97,113}, { 81, 89,113},
    {  0,  0, 65}, { 16,  0, 65}, { 32,  0, 65}, { 48,  0, 65}, { 65,  0, 65}, { 65,  0, 48}, { 65,  0, 32}, { 65,  0, 16},
    { 65,  0,  0}, { 65, 16,  0}, { 65, 32,  0}, { 65, 48,  0}, { 65, 65,  0}, { 48, 65,  0}, { 32, 65,  0}, { 16, 65,  0},
    {  0, 65,  0}, {  0, 65, 16}, {  0, 65, 32}, {  0, 65, 48}, {  0, 65, 65}, {  0, 48, 65}, {  0, 32, 65}, {  0, 16, 65},
    { 32, 32, 65}, { 40, 32, 65}, { 48, 32, 65}, { 56, 32, 65}, { 65, 32, 65}, { 65, 32, 56}, { 65, 32, 48}, { 65, 32, 40},
    { 65, 32, 32}, { 65, 40, 32}, { 65, 48, 32}, { 65, 56, 32}, { 65, 65, 32}, { 56, 65, 32}, { 48, 65, 32}, { 40, 65, 32},
    { 32, 65, 32}, { 32, 65, 40}, { 32, 65, 48}, { 32, 65, 56}, { 32, 65, 65}, { 32, 56, 65}, { 32, 48, 65}, { 32, 40, 65},
    { 44, 44, 65}, { 48, 44, 65}, { 52, 44, 65}, { 60, 44, 65}, { 65, 44, 65}, { 65, 44, 60}, { 65, 44, 52}, { 65, 44, 48},
    { 65, 44, 44}, { 65, 48, 44}, { 65, 52, 44}, { 65, 60, 44}, { 65, 65, 44}, { 60, 65, 44}, { 52, 65, 44}, { 48, 65, 44},
    { 44, 65, 44}, { 44, 65, 48}, { 44, 65, 52}, { 44, 65, 60}, { 44, 65, 65}, { 44, 60, 65}, { 44, 52, 65}, { 44, 48, 65},
    {  0,  0,  0}, {  0,  0,  0}, {  0,  0,  0}, {  0,  0,  0}, {  0,  0,  0}, {  0,  0,  0}, {  0,  0,  0}, {  0,  0,  0},
};

// ==================== color matching ====================
int closestColor(color c) {
    int best = 0;
    int bestDist = 999999;

    for (int i = 0; i < 256; i++) {
        int dr = c.r - vga_palette[i][0];
        int dg = c.g - vga_palette[i][1];
        int db = c.b - vga_palette[i][2];
        int dist = dr * dr + dg * dg + db * db;
        if (dist < bestDist) {
            bestDist = dist;
            best = i;
        }
    }

    if (best >= 248) return 255;
    return best;
}

// ==================== bmp -> tile ====================
void convertBMPtoTile(const char* bmpPath, const char* tileOutPath) {
    FILE* in = fopen(bmpPath, "rb");
    if (!in) { printf("can't open bmp input file\n"); return; }

    fseek(in, 10, SEEK_SET);
    uint32_t dataOffset;
    fread(&dataOffset, 4, 1, in);

    fseek(in, 18, SEEK_SET);
    int inputWidth, inputHeight;
    fread(&inputWidth, 4, 1, in);
    fread(&inputHeight, 4, 1, in);

    tileSizeX = (inputWidth > MAX_WIDTH) ? MAX_WIDTH : inputWidth;
    tileSizeY = (inputHeight > MAX_HEIGHT) ? MAX_HEIGHT : inputHeight;

    fseek(in, 54, SEEK_SET);
    color bmp_palette[256];
    for (int i = 0; i < 256; i++) {
        uint8_t bgr[4];
        fread(bgr, 1, 4, in);
        bmp_palette[i] = (color){ bgr[2], bgr[1], bgr[0] };
    }

    uint8_t* tempBuffer = malloc(inputWidth * inputHeight);
    if (!tempBuffer) {
        printf("couldn't allocate buffer\n");
        fclose(in);
        return;
    }

    fseek(in, dataOffset, SEEK_SET);
    for (int y = inputHeight - 1; y >= 0; y--) {
        for (int x = 0; x < inputWidth; x++) {
            uint8_t idx;
            fread(&idx, 1, 1, in);
            tempBuffer[y * inputWidth + x] = idx;
        }
        int pad = (4 - (inputWidth % 4)) % 4;
        fseek(in, pad, SEEK_CUR);
    }

    fclose(in);

    for (int y = 0; y < tileSizeY; y++) {
        for (int x = 0; x < tileSizeX; x++) {
            int srcX = x * inputWidth / tileSizeX;
            int srcY = y * inputHeight / tileSizeY;
            uint8_t idx = tempBuffer[srcY * inputWidth + srcX];
            color pix = bmp_palette[idx];
            int nearest = closestColor(pix);
            tileBuffer[y * tileSizeX + x] = nearest;
        }
    }

    free(tempBuffer);

    FILE* out = fopen(tileOutPath, "wb");
    if (!out) { printf("can't write tile output\n"); return; }

    fprintf(out, "%d %d ", tileSizeX, tileSizeY);
    fwrite(tileBuffer, 1, tileSizeX * tileSizeY, out);
    fclose(out);
}

// ==================== tile -> bmp ====================
void convertTileToBMP(const char* tilePath, const char* bmpOutPath) {
    FILE* in = fopen(tilePath, "rb");
    if (!in) {
        printf("can't open tile input\n");
        return;
    }

    fscanf(in, "%d %d ", &tileSizeX, &tileSizeY);
    if (tileSizeX > MAX_WIDTH || tileSizeY > MAX_HEIGHT) {
        printf("tile size too big, eat shit\n");
        fclose(in);
        return;
    }

    fread(tileBuffer, 1, tileSizeX * tileSizeY, in);
    fclose(in);

    int rowSize = (tileSizeX + 3) & ~3;
    int imageSize = rowSize * tileSizeY;
    int fileSize = 54 + 1024 + imageSize;

    FILE* out = fopen(bmpOutPath, "wb");
    if (!out) {
        printf("can't write bmp output\n");
        return;
    }

    // bmp header
    uint8_t header[54] = {0};
    header[0] = 'B';
    header[1] = 'M';
    *(uint32_t*)&header[2] = fileSize;
    *(uint32_t*)&header[10] = 54 + 1024;
    *(uint32_t*)&header[14] = 40;
    *(int32_t*)&header[18] = tileSizeX;
    *(int32_t*)&header[22] = tileSizeY;
    *(uint16_t*)&header[26] = 1;
    *(uint16_t*)&header[28] = 8;
    *(uint32_t*)&header[34] = imageSize;
    *(uint32_t*)&header[38] = 2835;
    *(uint32_t*)&header[42] = 2835;
    *(uint32_t*)&header[46] = 256;
    *(uint32_t*)&header[50] = 0;

    fwrite(header, 1, 54, out);

    // palette (256 * 4 = 1024 bytes)
    for (int i = 0; i < 256; i++) {
        uint8_t b = vga_palette[i][2];
        uint8_t g = vga_palette[i][1];
        uint8_t r = vga_palette[i][0];
        uint8_t zero = 0;
        fwrite(&b, 1, 1, out);
        fwrite(&g, 1, 1, out);
        fwrite(&r, 1, 1, out);
        fwrite(&zero, 1, 1, out);
    }

    // image data (bottom-up)
    for (int y = tileSizeY - 1; y >= 0; y--) {
        fwrite(&tileBuffer[y * tileSizeX], 1, tileSizeX, out);
        for (int i = 0; i < (rowSize - tileSizeX); i++) {
            fputc(0, out); // padding
        }
    }

    fclose(out);
}

// ==================== main ====================
int main(int argc, char* argv[]) {
    if (argc < 4) {
        printf("usage: converter --tile input.bmp output.til\n");
        printf("       converter --bmp input.til output.bmp\n");
        return 1;
    }

    if (strcmp(argv[1], "--tile") == 0) {
        convertBMPtoTile(argv[2], argv[3]);
    } else if (strcmp(argv[1], "--bmp") == 0) {
        convertTileToBMP(argv[2], argv[3]);
    } else {
        printf("invalid mode\n");
        return 1;
    }

    return 0;
}