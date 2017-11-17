#ifndef API_H
#define API_H

#include <juno.h>

typedef struct Juno Juno;

static uBYTE font[128][5*3];

void initfont();
void initSheet(Juno*);

int btn(Juno*, BYTE key);
int btnp(Juno*, BYTE key);

void pal(Juno*, BYTE col, BYTE ncol, BYTE offset);
void palt(Juno*, BYTE col, BYTE transparent);

char * input(Juno*);

void poke(Juno*, SHORT address, uBYTE data);
uBYTE peek(Juno*, SHORT address);

void pset(Juno*, short x, short y, BYTE color);

int pget(Juno*, short x, short y);

void spr(Juno*, BYTE sn, short x, short y, BYTE w, BYTE h, BYTE flip_h, BYTE flip_v);
void sspr(Juno *, BYTE sx, BYTE sy, BYTE sw, BYTE sh, short dx, short dy);

void line(Juno*, short x0, short y0, short x1, short y1, BYTE color);

void circ(Juno*, short x, short y, short radius, BYTE color);
void circfill(Juno*, short x, short y, short radius, BYTE color);

void rect(Juno*, short x0, short y0, short x1, short y1, BYTE color);
void rectfill(Juno*, short x0, short y0, short x1, short y1, BYTE color);

void printchar(Juno*, BYTE c, short x, short y, BYTE color);
void print(Juno*, const BYTE * text, short x, short y, BYTE color);

void color(Juno*, BYTE color);

void clip(Juno*, BYTE x, BYTE y, BYTE w, BYTE h);

void cursor(Juno*, BYTE x, BYTE y);

void camera(Juno*, short x, short y);

void clear(Juno*, BYTE color);

void flip(Juno*);

#endif // API_H