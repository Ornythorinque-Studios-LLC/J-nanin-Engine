/* courtesy of GPT-5 as i refuse to write ts myself */
#include <stdio.h>
#include <string.h>  /* strcmp, memset */

/* ===== config ===== */
#define MAX_WIDTH  128
#define MAX_HEIGHT 128
#define TILE_MAX   (MAX_WIDTH * MAX_HEIGHT)

/* ===== types ===== */
typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned long  u32;

typedef struct {
    u8 r, g, b;
} color;

/* ===== globals ===== */
static u8  tileBuffer[TILE_MAX];
static int tileSizeX = 0, tileSizeY = 0;

/* keep your giant vga palette definition in ONE place.
   declare it extern here, define it exactly as you had it elsewhere. */
const u8 vga_palette[256][3] = {
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


/* bmp palette we read from the file (256 entries of rgb) */
static color bmp_palette[256];

/* ===== little-endian helpers (portable on turbo c) ===== */
static u16 read_le16(FILE *f) {
    u8 b0, b1;
    if (fread(&b0, 1, 1, f) != 1) return 0;
    if (fread(&b1, 1, 1, f) != 1) return 0;
    return (u16)((u16)b0 | ((u16)b1 << 8));
}

static u32 read_le32(FILE *f) {
    u8 b0, b1, b2, b3;
    if (fread(&b0, 1, 1, f) != 1) return 0;
    if (fread(&b1, 1, 1, f) != 1) return 0;
    if (fread(&b2, 1, 1, f) != 1) return 0;
    if (fread(&b3, 1, 1, f) != 1) return 0;
    return (u32)((u32)b0 | ((u32)b1 << 8) | ((u32)b2 << 16) | ((u32)b3 << 24));
}

static void write_u16_le(FILE *f, u16 v) {
    u8 b0 = (u8)(v & 0xFF);
    u8 b1 = (u8)((v >> 8) & 0xFF);
    fwrite(&b0, 1, 1, f);
    fwrite(&b1, 1, 1, f);
}

static void write_u32_le(FILE *f, u32 v) {
    u8 b0 = (u8)(v & 0xFF);
    u8 b1 = (u8)((v >> 8) & 0xFF);
    u8 b2 = (u8)((v >> 16) & 0xFF);
    u8 b3 = (u8)((v >> 24) & 0xFF);
    fwrite(&b0, 1, 1, f);
    fwrite(&b1, 1, 1, f);
    fwrite(&b2, 1, 1, f);
    fwrite(&b3, 1, 1, f);
}

/* ===== color matching (use longs to avoid 16-bit overflow) ===== */
static int closestColor(color c) {
    int best = 0;
    long bestDist = 0x7FFFFFFFL; /* big number */
    int i;

    for (i = 0; i < 256; ++i) {
        long dr = (long)(int)c.r - (long)(int)vga_palette[i][0];
        long dg = (long)(int)c.g - (long)(int)vga_palette[i][1];
        long db = (long)(int)c.b - (long)(int)vga_palette[i][2];
        long dist = dr*dr + dg*dg + db*db;
        if (dist < bestDist) {
            bestDist = dist;
            best = i;
        }
    }
    if (best >= 248) return 255; /* keep that quirk if you meant it */
    return best;
}

/* ===== bmp -> tile (streaming, no malloc) =====
   reads only the rows we need and only the pixels we sample in those rows */
static void convertBMPtoTile(const char* bmpPath, const char* tileOutPath) {
    FILE *in = fopen(bmpPath, "rb");
    FILE *out;
    u32 dataOffset, headerSize, compression, imageSize;
    u32 paletteColors;
    long inputWidth, inputHeight;
    long row, neededRow, nextNeededRow;
    int pad;
    int y, x, dx;
    long srcY, srcXmap[MAX_WIDTH];

    if (!in) { printf("can't open bmp input file, ya walking error\n"); return; }

    /* --- parse header safely --- */
    /* signature 'BM' */
    if (fgetc(in) != 'B' || fgetc(in) != 'M') {
        printf("not a bmp, champ\n");
        fclose(in); return;
    }

    /* file size (skip), reserved (skip), data offset */
    (void)read_le32(in); /* file size */
    (void)read_le16(in); (void)read_le16(in); /* reserved */
    dataOffset = read_le32(in);

    /* dib header */
    headerSize = read_le32(in);
    if (headerSize < 40) { /* need BITMAPINFOHEADER or better */
        printf("unsupported bmp header, you fossil\n");
        fclose(in); return;
    }

    /* width, height (signed in bmp; height>0 => bottom-up) */
    inputWidth  = (long)read_le32(in);
    inputHeight = (long)read_le32(in);

    /* planes, bpp */
    (void)read_le16(in); /* planes */
    if (read_le16(in) != 8) {
        printf("only 8-bit indexed bmps, buddy\n");
        fclose(in); return;
    }

    compression = read_le32(in);
    if (compression != 0UL) {
        printf("no rle crap today. uncompressed only.\n");
        fclose(in); return;
    }

    imageSize = read_le32(in);         /* can be 0 for BI_RGB */
    (void)read_le32(in); (void)read_le32(in); /* xppm, yppm */
    paletteColors = read_le32(in);     /* 0 => 256 for 8bpp */
    (void)read_le32(in);               /* important colors */

    if (paletteColors == 0UL) paletteColors = 256UL;
    if (paletteColors < 256UL) {
        /* we'll still read 256 slots but missing ones become black */
    }

    /* clamp output size */
    tileSizeX = (inputWidth  > MAX_WIDTH)  ? MAX_WIDTH  : (int)inputWidth;
    tileSizeY = (inputHeight > MAX_HEIGHT) ? MAX_HEIGHT : (int)inputHeight;
    if (tileSizeX <= 0 || tileSizeY <= 0) {
        printf("invalid bmp dims, soggy noodle\n");
        fclose(in); return;
    }

    /* read palette (BGRA) at end of header area; seek to palette start */
    /* after the DIB header, we're already positioned at palette in most bmps */
    {
        u32 i;
        u8 bgrx[4];
        for (i = 0; i < 256UL; ++i) {
            if (fread(bgrx, 1, 4, in) != 4) {
                /* if palette shorter than 256, fill rest with zero */
                for (; i < 256UL; ++i) {
                    bmp_palette[i].r = bmp_palette[i].g = bmp_palette[i].b = 0;
                }
                break;
            }
            bmp_palette[i].r = bgrx[2];
            bmp_palette[i].g = bgrx[1];
            bmp_palette[i].b = bgrx[0];
        }
    }

    /* jump to pixel array explicitly */
    fseek(in, (long)dataOffset, SEEK_SET);

    /* precompute column sampling map: srcX = floor(dx * inW / outW) */
    for (dx = 0; dx < tileSizeX; ++dx) {
        /* use long to avoid overflow */
        srcXmap[dx] = ((long)dx * (long)inputWidth) / (long)tileSizeX;
    }

    /* row padding to 4-byte boundary */
    pad = (int)((4L - (inputWidth % 4L)) % 4L);

    /* we’ll read rows bottom->top (file order). we only process rows equal to
       srcY = floor(y * inH / outH). as fileRow increases (0=bottom), we bump y. */
    y = 0;
    nextNeededRow = (long)y * (long)inputHeight / (long)tileSizeY;

    for (row = 0; row < inputHeight; ++row) {
        if (row == nextNeededRow) {
            /* process this row: pull just the pixels we need */
            int dxWant = 0;
            long nextX = srcXmap[dxWant];
            long xPos;

            for (xPos = 0; xPos < inputWidth; ++xPos) {
                int ch = fgetc(in);
                if (ch == EOF) { printf("truncated bmp, clown shoes\n"); fclose(in); return; }
                if (xPos == nextX) {
                    color pix = bmp_palette[(u8)ch];
                    tileBuffer[(tileSizeY - 1 - y) * tileSizeX + dxWant] = (u8)closestColor(pix);
                    ++dxWant;
                    if (dxWant >= tileSizeX) {
                        /* consume the rest of the row into the void */
                        long skip = (inputWidth - 1 - xPos);
                        if (skip > 0) fseek(in, (long)skip, SEEK_CUR);
                        break;
                    }
                    nextX = srcXmap[dxWant];
                }
            }
            /* skip row padding */
            if (pad) fseek(in, (long)pad, SEEK_CUR);

            /* advance to next needed row */
            ++y;
            printf("\rconverting... %3d%%", (y * 100) / tileSizeY);
            if (y >= tileSizeY) {
                /* we got all output rows, skip the rest fast */
                long remain = (long)(inputHeight - 1 - row);
                long stride = (long)inputWidth + (long)pad;
                if (remain > 0) fseek(in, remain * stride, SEEK_CUR);
                break;
            }
            nextNeededRow = (long)y * (long)inputHeight / (long)tileSizeY;
        } else {
            /* skip entire row + padding */
            long stride = (long)inputWidth + (long)pad;
            fseek(in, stride, SEEK_CUR);
        }
    }

    fclose(in);

    /* write tile (ascii header + raw indices) */
    out = fopen(tileOutPath, "wb");
    if (!out) { printf("can't write tile output, butterfingers\n"); return; }

    /* simplest: text header "w h " then data */
    fprintf(out, "%d %d ", tileSizeX, tileSizeY);
    fwrite(tileBuffer, 1, (size_t)(tileSizeX * tileSizeY), out);
    fclose(out);
}

/* ===== tile -> bmp (no dynamic alloc) ===== */
static void convertTileToBMP(const char* tilePath, const char* bmpOutPath) {
    FILE *in = fopen(tilePath, "rb");
    FILE *out;
    int i, y;
    int rowSize, imageSize;
    u32 fileSize;

    if (!in) { printf("can't open tile input, bozo\n"); return; }

    if (fscanf(in, "%d %d ", &tileSizeX, &tileSizeY) != 2) {
        printf("bad tile header, champ\n"); fclose(in); return;
    }
    if (tileSizeX > MAX_WIDTH || tileSizeY > MAX_HEIGHT || tileSizeX <= 0 || tileSizeY <= 0) {
        printf("tile size too big, eat sand\n");
        fclose(in); return;
    }

    if ((int)fread(tileBuffer, 1, (size_t)(tileSizeX * tileSizeY), in) != tileSizeX * tileSizeY) {
        printf("tile data truncated, noodle arms\n"); fclose(in); return;
    }
    fclose(in);

    /* bmp layout */
    rowSize   = (tileSizeX + 3) & ~3;        /* padded to 4 */
    imageSize = rowSize * tileSizeY;
    fileSize  = 14 + 40 + 1024 + (u32)imageSize; /* file hdr + dib + palette + pixels */

    out = fopen(bmpOutPath, "wb");
    if (!out) { printf("can't write bmp output, butter troll\n"); return; }

    /* --- file header (14 bytes) --- */
    fputc('B', out); fputc('M', out);
    write_u32_le(out, fileSize);
    write_u16_le(out, 0); write_u16_le(out, 0);            /* reserved */
    write_u32_le(out, 14 + 40 + 1024);                     /* pixel data offset */

    /* --- dib header (BITMAPINFOHEADER, 40 bytes) --- */
    write_u32_le(out, 40);                                 /* header size */
    write_u32_le(out, (u32)tileSizeX);                     /* width */
    write_u32_le(out, (u32)tileSizeY);                     /* height (positive => bottom-up) */
    write_u16_le(out, 1);                                  /* planes */
    write_u16_le(out, 8);                                  /* bpp */
    write_u32_le(out, 0);                                  /* compression (BI_RGB) */
    write_u32_le(out, (u32)imageSize);                     /* image size */
    write_u32_le(out, 2835);                               /* x ppm */
    write_u32_le(out, 2835);                               /* y ppm */
    write_u32_le(out, 256);                                /* colors used */
    write_u32_le(out, 0);                                  /* important */

    /* --- palette (bgra) --- */
    for (i = 0; i < 256; ++i) {
        u8 b = vga_palette[i][2];
        u8 g = vga_palette[i][1];
        u8 r = vga_palette[i][0];
        u8 z = 0;
        fwrite(&b, 1, 1, out);
        fwrite(&g, 1, 1, out);
        fwrite(&r, 1, 1, out);
        fwrite(&z, 1, 1, out);
    }

    /* --- pixel data (bottom-up) --- */
    for (y = tileSizeY - 1; y >= 0; --y) {
        int written = (int)fwrite(&tileBuffer[y * tileSizeX], 1, (size_t)tileSizeX, out);
        int pad = rowSize - tileSizeX;
        while (pad-- > 0) fputc(0, out);
        if (y == 0) break; /* prevent y from going negative on 16-bit int loop */
        (void)written;
    }

    fclose(out);
}

/* ===== main ===== */
int main(int argc, char* argv[]) {
    if (argc < 4) {
        printf("usage: tileconv -t input.bmp output.tile\n");
        printf("       tileconv -b  input.tile output.bmp\n");
        return 1;
    }

    if (strcmp(argv[1], "-t") == 0) {
        convertBMPtoTile(argv[2], argv[3]);
    } else if (strcmp(argv[1], "-b") == 0) {
        convertTileToBMP(argv[2], argv[3]);
    } else {
        printf("invalid mode, ya confused turnip\n");
        return 1;
    }

    return 0;
}
