#ifndef VIDEO_H
#define VIDEO_H

#define WIDTH 320
#define HEIGHT 200

void toggleVideoMode(void);
void plotPixel(int pixelX, int pixelY, unsigned char pixelColor);
int fetchPixelColor(int pixelX, int pixelY);

#endif;
