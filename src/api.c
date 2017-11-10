#include <api.h>

BYTE * _keys[6] = {"left", "right", "up", "down", "x", "c"};

int btn(BYTE key) {
    return console->keys[SDL_GetScancodeFromName(_keys[key])];
}

void poke(SHORT address, BYTE data) {

    if (address < 0 || address > MEM_SIZE) {
        printf("Endereço inválido\n");
        return;
    }
    //printf("without mid %d\n", data);
    data = MID(data, -127, 127);
    //printf("with mid %d\n", data);

    console->memory[address] = (data&0xff);
}

BYTE peek(SHORT address) {
    if (address < 0 || address > MEM_SIZE) {
        return 0;
    }
    return console->memory[address] & 0xff;
}

void pset(short x, short y, BYTE color) {
    short camx = peek(0x5f28) | ((peek(0x5f29)<<8));
    short camy = peek(0x5f2a) | ((peek(0x5f2b)<<8));
    x -= camx;
    y -= camy;
    short xx = MID(floor(x/2), 0, 64);
    short yy = MID(y, 0, 128);

    SHORT address = ((xx%64)+(yy*64));
    BYTE col = console->gfx[address];

    BYTE clx = peek(0x5f20);
    BYTE cly = peek(0x5f21);
    BYTE clw = peek(0x5f22);
    BYTE clh = peek(0x5f23);

    //printf("%d %d %d %d\n", clx, cly, clw, clh);

    if (x >= clx && y >= cly &&
        x <= clw && y <= clh) {
        if (x%2) {
            console->gfx[address] = ((col&0xf)) | (color<<4);
        } else {
            console->gfx[address] = (col&0xf0) | color;
        }
    }
}

int pget(short x, short y) {
    //short camx = (peek(0x5f28))
    if (x < 0 || x > 127 ||
        y < 0 || y > 127) {
            return 0;
        }
    short xx = floor(x/2);
    short yy = y;

    SHORT address = ((xx%64)+(yy*64));
    BYTE col = console->gfx[address];

    if (x%2) {
        return (col&0xf0)>>4;
    } else {
        return col&0xf;
    }
}

void line(short x0, short y0, short x1, short y1, BYTE color) { 

    int dx = x1 - x0;
    int dy = y1 - y0;
    int steps = MAX(abs(dx), abs(dy));
    float xinc = dx / (float) steps;
    float yinc = dy / (float) steps;

    pset(x0, y0, color);
    float x = x0;
    float y = y0;
    for (int v = 0; v < steps; v++) {
        x += xinc;
        y += yinc;
        pset(x, y, color);
    }
}

void circ(short x, short y, short radius, BYTE color) {
    short xx = -radius;
    short yy = 0;
    short r = radius;
    short dx = 1;
    short dy = 1;
    short err = 2-2*r;

    do {
        pset(x-xx, y+yy, color);
        pset(x-yy, y-xx, color);
        pset(x+xx, y-yy, color);
        pset(x+yy, y+xx, color);
        r = err;
        if (r <= yy) err += ++yy*2+1;
        if (r > xx || err > yy) err += ++xx*2+1;
    } while (xx < 0);

}

void circfill(short x, short y, short radius, BYTE color) {
    short xx = -radius;
    short yy = 0;
    short r = radius;
    short dx = 1;
    short dy = 1;
    short err = 2-2*r;

    do {
        line(x - xx, y + yy, x + xx, y + yy, color);
		line(x - yy, y + xx, x + yy, y + xx, color);
		line(x + xx, y - yy, x - xx, y - yy, color);
		line(x + yy, y - xx, x - yy, y - xx, color);
        r = err;
        if (r <= yy) err += ++yy*2+1;
        if (r > xx || err > yy) err += ++xx*2+1;
    } while (xx < 0);
    
}

void rect(short x, short y, short w, short h, BYTE color) {
    line(x+1,y,x+w,y,color);
    line(x+w,y+1,x+w,y+h,color);
    line(x+w-1,y+h,x,y+h,color);
    line(x,y+h-1,x,y,color);
}

void rectfill(short x, short y, short w, short h, BYTE color) {
    for (int yy=y; yy <= (y+h); yy++) {
        line(x,yy,x+w,yy,color);
    }
}

void clip(BYTE x, BYTE y, BYTE w, BYTE h) {
    w = MID(x+w, -127, 127);
    h = MID(y+h, -127, 127);
    poke(0x5f20, x);
    poke(0x5f21, y);
    poke(0x5f22, w);
    poke(0x5f23, h);
}

void camera(short x, short y) {
    poke(0x5f28, x & 0xff);
    poke(0x5f29, (x & 0xff00) >> 8);
    poke(0x5f2a, y & 0xff);
    poke(0x5f2b, (y & 0xff00) >> 8);
}

void color(BYTE color) {
    poke(0x5f25,color);
}

void clear(BYTE color) {
    SDL_RenderClear(console->render);
    for (int addr = 0; addr < 0x2000; addr++) {
        console->gfx[addr] = (color<<4) | color;
    }
}

void flip() {
    for (int addr = 0; addr < 0x2000; addr++) {
        short xx = (addr*2)%128;
        short yy = floor(addr/64);

        BYTE colorPos = console->gfx[addr];
        BYTE color1 = colorPos & 0xf;
        BYTE color2 = (colorPos&0xf0)>>4;

        SDL_SetRenderDrawColor(console->render,
                               console->palette[color1].r,
                               console->palette[color1].g,
                               console->palette[color1].b,
                               255);
        SDL_RenderDrawPoint(console->render, 8+xx, 3+yy);

        SDL_SetRenderDrawColor(console->render,
                               console->palette[color2].r,
                               console->palette[color2].g,
                               console->palette[color2].b,
                               255);
        SDL_RenderDrawPoint(console->render, 8+xx+1, 3+yy);
    }
}