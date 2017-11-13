#ifndef API_H
#define API_H

#include <juno.h>

static uBYTE font[128][5*3];

void initfont();

int btn(BYTE key);
int btnp(BYTE key);

void pal(BYTE col, BYTE ncol, BYTE offset);
void palt(BYTE col, BYTE transparent);

char * input();

void poke(SHORT address, uBYTE data);
uBYTE peek(SHORT address);

void pset(short x, short y, BYTE color);

int pget(short x, short y);

void line(short x0, short y0, short x1, short y1, BYTE color);

void circ(short x, short y, short radius, BYTE color);
void circfill(short x, short y, short radius, BYTE color);

void rect(short x0, short y0, short x1, short y1, BYTE color);
void rectfill(short x0, short y0, short x1, short y1, BYTE color);

void printchar(BYTE c, short x, short y, BYTE color);
void print(const BYTE * text, short x, short y, BYTE color);

void color(BYTE color);

void clip(BYTE x, BYTE y, BYTE w, BYTE h);

void cursor(BYTE x, BYTE y);

void camera(short x, short y);

void clear(BYTE color);

void flip();

#endif // API_H