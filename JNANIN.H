#ifndef JNANIN_H
#define JNANIN_H

struct {
	const char* name;
	int pixels[320][200];
} Game;

void gameInit(Game* game);
void gameFree(Game* game);

#endif