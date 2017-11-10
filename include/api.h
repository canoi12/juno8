#ifndef API_H
#define API_H

#include <juno8.h>

int btn(BYTE key);
int btnp(BYTE key);

void poke(SHORT address, BYTE data);

BYTE peek(SHORT address);

void pset(short x, short y, BYTE color);

int pget(short x, short y);

void line(short x0, short y0, short x1, short y1, BYTE color);

void circ(short x, short y, short radius, BYTE color);
void circfill(short x, short y, short radius, BYTE color);

void rect(short x, short y, short w, short h, BYTE color);
void rectfill(short x, short y, short w, short h, BYTE color);

void color(BYTE color);

void clip(BYTE x, BYTE y, BYTE w, BYTE h);

void camera(short x, short y);

void clear(BYTE color);

void flip();

#endif // API_H