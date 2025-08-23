#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <conio.h>
#include <string.h>
#include <dos.h>

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

#define PIXEL_COUNT (tileSizeX * tileSizeY)

#define WIDTH 320
#define HEIGHT 200

/* macro / preprocessor abuse */
#define calcCoordValueX(coord) (boundX + ((coord) * scale))
#define calcCoordValueY(coord) (boundY + ((coord) * scale))
#define calcCoordIndexX(pixelX) (((pixelX) - boundX) / scale)
#define calcCoordIndexY(pixelY) (((pixelY) - boundY) / scale)

#define checkCursBounds(cursorX, cursorY) ((cursorX) >= boundX && (cursorY) >= boundY && (cursorX) <= (maxX - scale) && (cursorY) <= (maxY - scale))

#define plotPixel(x, y, color) (video_memory[((y) * WIDTH) + (x)] = (color))
#define fetchPixelColor(x, y) (video_memory[((y) * WIDTH) + (x)])

#define fillScreen(color) (memset((unsigned char*)video_memory, color, WIDTH * HEIGHT))

/* vars. */
int tileSizeX, tileSizeY, maxX, maxY, boundX, boundY, drawSizeX, drawSizeY;
int cursX, cursY, xCoord, yCoord, xCoordValue, yCoordValue;
static int scale = 2;
static unsigned char prevPixelColor = 0;
static int prevColorPickerPixelColor = 0;
unsigned char* tileBuffer;
static unsigned char color = 15;
static int prevCursX = -1;
static int prevCursY = -1;
static int mode = 0;

/* mouse stuff */
static int mouseX = 0;
static int mouseY = 0;
static int leftClicked;
static int rightClicked;
static int prevMouseX = -1;
static int prevMouseY = -1;
static unsigned char prevMousePixelColor = 0;

int isToggled = 0;

unsigned char far *video_memory = (unsigned char far*)0xA0000000L;

void toggleVideoMode(void) {
	union REGS regs;
	if (!isToggled) {
		regs.x.ax = 0x13;
	} else {
		regs.x.ax = 0x03;
	}
	int86(0x10, &regs, &regs);
	isToggled = !isToggled;
}

void pollMouse(void) {
	union REGS regs;
	regs.x.ax = 0x03;
	int86(0x33, &regs, &regs);
	mouseX = (regs.x.cx / 2);
	mouseY = regs.x.dx;
	if (regs.x.bx & 1) { leftClicked = 1; } else { leftClicked = 0; }
	if (regs.x.bx & 2) { rightClicked = 1; } else { rightClicked = 0; }
	if (mouseX > 320) { mouseX = 320; }
	if (mouseY > 200) { mouseY = 200; }
}

void drawMouse(void) {
	if (prevMouseX == mouseX && prevMouseY == mouseY) { return; }

	if (prevMouseX != -1 && prevMouseY != -1) {
		plotPixel(prevMouseX, prevMouseY, prevMousePixelColor);
	}

	prevMousePixelColor = fetchPixelColor(mouseX, mouseY);

	plotPixel(mouseX, mouseY, prevMousePixelColor - 128);

	prevMouseX = mouseX;
	prevMouseY = mouseY;
}

void drawRect(int x, int y, int width, int height, int color) {
	int i, j;
	for (i = 0; i < width + 1; i++) {
		plotPixel(x + i, y, color);
		plotPixel(x + i, y + height, color);
	}
	for (j = 0; j < height; j++) {
		plotPixel(x, y + j, color);
		plotPixel(x + width, y + j, color);
	}
 }

void setBounds(void) {
	drawSizeX = tileSizeX * scale;
	drawSizeY = tileSizeY * scale;
	boundX = (WIDTH / 2) - drawSizeX / 2;
	boundY = (HEIGHT / 2) - drawSizeY / 2;
	maxX = boundX + drawSizeX;
	maxY = boundY + drawSizeY;
}

void drawBounding(void) {
	int i, e;
	const int bordThick = 2;
	setBounds();
	for (e = 1; e < bordThick + 1; e++) { 
		for (i = 0; i < drawSizeX + e + bordThick; i++) { 
			plotPixel(boundX + i - e, maxY - e+ bordThick, 8);
			plotPixel(boundX + i - e, boundY - e, 8); 
		}
		for (i = 0; i < drawSizeY + e + bordThick; i++) { 
			plotPixel(maxX - e + bordThick, boundY + i - e, 8); 
			plotPixel(boundX - e, boundY + i - e, 8); 
		}
	}
}

void drawPix(int pixelX, int pixelY, unsigned char pixelColor) {
	if (pixelX >= boundX && pixelY >= boundY && pixelX <= maxX && pixelY <= maxY - 1) {
		plotPixel(pixelX, pixelY, pixelColor);
	}
}

int checkBounds(int scaleCheck) {
	int originalScaleCheck = scale;
	scale = scaleCheck;
	setBounds();
	scale = originalScaleCheck;
	if (drawSizeX > 160) return 1; /* x too large */
	if (drawSizeY > 160) return 2; /* y too large */
	if ((drawSizeX * drawSizeY) > (160 * 160)) return 3; /* what */
	return 0;
}

void scaleDown(int x, int y) {
	int oldX, oldY, oldScale;
	if (x > 160) x = 160;
	if (y > 160) y = 160;
	oldX = tileSizeX; oldY = tileSizeY; oldScale = scale;
	tileSizeX = x; tileSizeY = y;
	setBounds();
	while (checkBounds(scale)) {
		scale--;
		if (scale < 1) {
			tileSizeX = oldX;
			tileSizeY = oldY;
			scale = oldScale;
			break;
		}
	}
	setBounds();
}

void clearTile(void) {
	int i, j;
	for (i = 0; i < 164; i++) {
		for (j = 0; j < 164; j++) {
			plotPixel(j + 78, i + 18, 255);
		}
	}
}

void drawCurs(int mode) {
	if (!mode) {
		if (cursX < boundX) cursX = boundX;
		if (cursY < boundY) cursY = boundY;
		if (cursX > maxX - scale) cursX = maxX - scale;
		if (cursY > maxY - scale) cursY = maxY - scale;

		if (prevCursX == cursX && prevCursY == cursY) { return; }

		if (prevCursX != -1 && prevCursY != -1) {
			drawRect(prevCursX, prevCursY, scale - 1, scale - 1, prevPixelColor);
		}

		prevPixelColor = fetchPixelColor(cursX, cursY);

		drawRect(cursX, cursY, scale - 1, scale - 1, prevPixelColor - 127);

		prevCursX = cursX;
		prevCursY = cursY;
	}
	
	if (mode) {
		cursX = ((cursX / 3) * 3) + (1);
		cursY = ((cursY / 3) * 3) + (1);

		if (prevCursX == cursX && prevCursY == cursY) {
			return;
		}

		if (prevCursX != -1 && prevCursY != -1) {
			drawRect(prevCursX, prevCursY, 2, 2, prevColorPickerPixelColor);
		}
			
		prevColorPickerPixelColor = fetchPixelColor(cursX, cursY);

		if (!(cursX >= 23 && cursY >= 64)) { color = prevColorPickerPixelColor; }
		else { color = 255; }

		drawRect(cursX, cursY, 2, 2, 15);

		prevCursX = cursX;
		prevCursY = cursY;
	}
}

void resizeTile(int width, int height) {
	free(tileBuffer);
	tileSizeX = width;
	tileSizeY = height;
	setBounds();
	tileBuffer = (unsigned char*)malloc(PIXEL_COUNT);
	if (tileBuffer == NULL) {
		toggleVideoMode();
		gotoxy(0, 0);
		printf("Unable to allocate memory!\n Press any key to quit.\n");
		getch();
		exit(1);
	}
	memset(tileBuffer, 255, PIXEL_COUNT);
}

void redrawTile(void) {
    int i, j, k, l;
    unsigned char color;

    cursX = boundX;
    cursY = boundY;

    for (j = 0; j < tileSizeY; j++) {
        for (i = 0; i < tileSizeX; i++) {
            int x = calcCoordValueX(i);
            int y = calcCoordValueY(j);

            color = tileBuffer[(j * tileSizeX) + i];

            for (k = 0; k < scale; k++) {
                for (l = 0; l < scale; l++) {
                    drawPix(x + l, y + k, color);
                }
            }
        }
    }
}

void drawColorSquare(void) {
	int i, j, k, l;
	unsigned char color = 0;
	for (i = 0; i < 16; i++) {
		for (j = 0; j < 16; j++) {
			for (k = 0; k < 3; k++) {
				for (l = 0; l < 3; l++) {
					plotPixel(1 + l + (j * 3), k + 19 + (i * 3), color);
				}
			}
			color++;
		}
	}
	plotPixel(1 + l + (j * 3), k + 19 + (i * 3), 254);
	drawRect(0, 18, 49, 49, 15);
}

char fetchKey(void) {
	unsigned char key = 0;
	if (kbhit()) {
		key = getch();
		if (key == 0 || key == 224) {
			key = getch();
		}
	} 
	else key = 0;
	return key;
}

void drawInit(void) {
	setBounds();

	mode = 0;

	cursX = boundX;
	cursY = boundY;

	xCoord = 0;
	yCoord = 0;

	drawBounding();
	redrawTile();
	drawColorSquare();
	printf(" (S)ave (L)oad (R)esize (C)lear (Q)uit   +/- (zoom)  ` (pick color)  (P)alette");
}

void saveTile(void) {
	char path[100];
	int i;
	FILE* file;
	toggleVideoMode();
	gotoxy(0, 0);
	printf("Where would you like to save to?\n");
	scanf("%s", &path);
	file = fopen(path, "wb");
	if (file == NULL) { printf("Failed to open file!\nPress any key to continue."); getch(); }
	else {
		fprintf(file, "%d %d ", tileSizeX, tileSizeY);
		fwrite(tileBuffer, 1, PIXEL_COUNT, file);
		fclose(file);
	}
	toggleVideoMode();
	drawInit();
}

void loadTile(void) {
	char path[100];
	int i;
	FILE* file;
	toggleVideoMode();
	gotoxy(0, 0);
	printf("Where would you like to load from?\n");
	scanf("%s", &path);
	file = fopen(path, "rb");
	if (file == NULL) { printf("Failed to open file!\nPress any key to continue."); getch(); }
	else {
		fscanf(file, "%d %d ", &tileSizeX, &tileSizeY);
		resizeTile(tileSizeX, tileSizeY);
		scaleDown(tileSizeX, tileSizeY);
		fread(tileBuffer, 1, PIXEL_COUNT, file);
		fclose(file);
	}
	toggleVideoMode();
	drawInit();
}

void getTileSize(void) {
	int oldX, oldY;
	oldX = tileSizeX; oldY = tileSizeY;
	toggleVideoMode();
	gotoxy(0, 0);
	printf("Set X value:\n");
	scanf("%d", &tileSizeX);
	printf("Set Y value:\n");
	scanf("%d", &tileSizeY);
	if (tileSizeX < 2 || tileSizeX > 160 || tileSizeY < 2 || tileSizeY > 160) {
		printf("Invalid values. \n");
		getch();
		tileSizeX = oldX; tileSizeY = oldY;
	}
	else scale = 10; resizeTile(tileSizeX, tileSizeY); scaleDown(tileSizeX, tileSizeY);
	toggleVideoMode();
	drawInit();
}

void keyHandle(void) {
	char key;
	int i, j;
	key = fetchKey();
	if (key == NULL) return;
                                 
	if (key == 'q') { 
		free(tileBuffer);
		toggleVideoMode(); 
		exit(0);
	}

	if (!mode) {
		if (key == '=' || key == '+') { 
			if (checkBounds(scale + 1) != 0) { setBounds(); }
			else { 
				clearTile(); scale++; drawBounding(); redrawTile(); 
				cursX = calcCoordValueX(xCoord); cursY = calcCoordValueY(yCoord); 
				prevCursX = cursX; prevCursY = cursY;
			}
		}

		if ((key == '-' || key == '_') && scale > 1) { 
			clearTile(); scale--; drawBounding(); redrawTile(); 
			cursX = calcCoordValueX(xCoord); cursY = calcCoordValueY(yCoord); 
			prevCursX = cursX; prevCursY = cursY;
		}

		if (key == 'c') { clearTile(); drawBounding(); memset(tileBuffer, 255, PIXEL_COUNT); }

		if (key == ' ' || key == 'e') {
			unsigned clickColour = color;
			if (key == 'e') clickColour = 255;
			for (i = 0; i < scale; i++) {
				for (j = 0; j < scale; j++) {
					drawPix(j + cursX, i + cursY, clickColour);
					prevPixelColor = (unsigned char)clickColour;
				}
			}
			tileBuffer[(yCoord * tileSizeX) + xCoord] = clickColour;
		}

		if (key == '`' || key == '~') {
			color = tileBuffer[(yCoord * tileSizeX) + xCoord];
		}

		if (key == 'f') {
			memset(tileBuffer, color, PIXEL_COUNT);
			redrawTile();
		}
	}

	if (key == KEY_UP) { 
		if (!mode && checkCursBounds(cursX, cursY - scale)) { cursY -= scale; yCoord--; } 
		else if (cursY > 19 && mode) cursY -= 3;
	}
	if (key == KEY_DOWN) {
		if (!mode && checkCursBounds(cursX, cursY + scale)) { cursY += scale; yCoord++; }
		else if (cursY < 64 && mode) cursY += 3;
	}
	if (key == KEY_LEFT) { 
		if (!mode && checkCursBounds(cursX - scale, cursY)) { cursX -= scale; xCoord--; } 
		else if (cursX > 1 && mode) cursX -= 3;
	}
	if (key == KEY_RIGHT) { 
		if (!mode && checkCursBounds(cursX + scale, cursY)) { cursX += scale; xCoord++; }
		else if (cursX < 46 && mode) cursX += 3;
		else;
	}
	
	if (key == 'p') { 
		static int oldPosX;
		static int oldPosY;
		mode = !mode; 
		if (mode) { 
			oldPosX = cursX; oldPosY = cursY; 
			cursX = 1; cursY = 19; 
			prevCursX = 1; prevCursY = 19;
			prevColorPickerPixelColor = 0;
			drawRect(prevCursX, prevCursY, 2, 2, 15);
		}
		else { 
			drawRect(prevCursX, prevCursY, 2, 2, prevColorPickerPixelColor);
			cursX = oldPosX; cursY = oldPosY; 
			prevCursX = oldPosX; prevCursY = oldPosY; 
		}
	}

	if (key == 's') saveTile();
	if (key == 'l') loadTile();
	if (key == 'r') getTileSize();

	drawCurs(mode);
}

int main(void) {
	int mouseEnabled = 1;
	union REGS regs;
	regs.x.ax = 0x00;
	int86(0x33, &regs, &regs);
	mouseEnabled = 1;
	if (regs.x.ax != 0xFFFF) mouseEnabled = 0;

	tileSizeX = 16;
	tileSizeY = 16;
	scale = 10;
	scaleDown(16, 16);

	toggleVideoMode();

	tileBuffer = (unsigned char*)malloc(PIXEL_COUNT);
	if (tileBuffer == NULL) {
		toggleVideoMode();
		gotoxy(0, 0);
		printf("Unable to allocate memory!\n Press any key to quit.\n");
		getch();
		return 1;
	}
	memset(tileBuffer, 255, PIXEL_COUNT);
	drawInit();

	while(1) {
		int i, j;
  		if (mouseEnabled) {
			pollMouse();

			if (leftClicked && (mouseX > 0 && mouseX < 49 && mouseY > 18 && mouseY < 67)) {
				if (!(mouseX >= 23 && mouseY >= 64)) { color = prevMousePixelColor; }
				else { color = 255; }
			}

			if (mouseX >= boundX && mouseX <= maxX && mouseY >= boundY && mouseY <= maxY && !mode) {
				if ((leftClicked || rightClicked)) {
					int i, j;
					unsigned clickColour = color;
					if (rightClicked) clickColour = 255;
					cursX = calcCoordValueX(((mouseX - boundX) / scale));
					cursY = calcCoordValueY(((mouseY - boundY) / scale));
					xCoord = calcCoordIndexX(mouseX);
					yCoord = calcCoordIndexY(mouseY);
					for (i = 0; i < scale; i++) {
						for (j = 0; j < scale; j++) {
							drawPix(j + cursX, i + cursY, clickColour);
							prevMousePixelColor = (unsigned char)clickColour;
						}
					}
					if (leftClicked) tileBuffer[(yCoord * tileSizeX) + xCoord] = color;
					if (rightClicked) tileBuffer[(yCoord * tileSizeX) + xCoord] = 255;
					drawBounding();
				}
			}
			drawMouse();
		}

		keyHandle();

		drawRect(0, 74, 9, 9, 15);

		for (i = 0; i < 8; i++) {
			for (j = 0; j < 8; j++) {
				plotPixel(j + 1, i + 75, color);
			}
		}

		gotoxy(1, 12);
		if (color != 255) printf("%d     ", color);
		else { printf("Clear"); }
	}
}