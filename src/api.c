#include <api.h>

BYTE * _keys[6] = {"left", "right", "up", "down", "x", "c"};
/*BYTE * font = {
    ""
};*/
static uBYTE * letters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!?[](){}.,;:<>+=%#^*~/\\|$@&`\"'-_";

void initfont() {
    FILE * fp = fopen("font", "r");
    char * line = NULL;
    size_t len;
    ssize_t read;
    //char * curchar = letters;

    printf("%d\n", font[48][0]);

    if (!fp) {
        printf("Arquivo de fontes não encontrado\n");
        exit(1);
    }
    int offset = 0;
    while((read=getline(&line,&len,fp)) != -1) {
        char * curchar = letters;
        char * c = line;
        for(int i = 0; i < 94; i++) {
            for (int j = 0; j < 3; j++) {
                if (*line != '\n') {
                    font[*curchar][j+offset] = (*c)-'0';
                    c++;
                }
            }
            curchar++;
        }
        //free(c);
        //free(curchar);
        //curchar = letters;
        offset+=3;
    }
    printf("%c\n", font[48][0]);
    printf("Fontes carregadas\n");
    //if(line) free(line);
    fclose(fp);
}

void pal(BYTE col, BYTE ncol, BYTE offset) {
    if (col < 0 || col > 15) {
        return;
    }
    BYTE color = peek(0x5f00+col+(0x10*offset));
    color = (color&0xf0) | (ncol&0xf);
    poke(0x5f00+col+(0x10*offset), color);
}

void palt(BYTE col, BYTE transparency) {
    if (col < 0 || col > 15) {
        return;
    }
    BYTE color = peek(0x5f00+col);
    color = (transparency<<7) | (color&0xf);
    poke(0x5f00+col, color);
}

char * input() {
    //char * ret;
    //printf("okeh\n");
    if (console->event.type == SDL_TEXTINPUT) {
        //if (strcmp(console->event.text.text, console->console->last_char)) {
        //size_t len = strlen(textptr);
        /*if (len < 32) {
            strcat(textptr, console->event.text.text);
        }*/
        char * ret = console->event.text.text;
        //strcpy(ret, console->event.text.text);
        //printf("%s\n", console->event.text.text);
        //}
        //printf("%c\n", *(console->event.text.text));
        //strcpy(console->console->last_char, console->event.text.text);
        return ret;
    } else {
        //strcpy(ret, "");
    }
    //printf("%s\n", ret);
    /*if (keyPressed("backspace")) {
        size_t len = strlen(textptr);
        if (len > 0) {
            char aux[len];
            strncpy(aux, textptr, len-1);
            aux[len-1] = '\0';
            strcpy(textptr, aux);
        }
    }
    if (keyPressed("return")) {
        if (console->state == CONSOLE) {
            if (!luaL_dostring(console->l, textptr)) {
                printf("xuxu beleza\n");
            } else {
                printf("eita\n");
            }
            //lua_
            strcpy(textptr, "");
            //clear(0);
        }
    }*/
    char * ret = "";
    return ret;
}

int btn(BYTE key) {
    return keyDown(_keys[key]);
}

int btnp(BYTE key) {
    return keyPressed(_keys[key]);
}

void poke(SHORT address, uBYTE data) {

    if (address < 0 || address > MEM_SIZE) {
        printf("Endereço inválido\n");
        return;
    }
    //printf("without mid %d\n", data);
    data = MID(data, 0, 255);
    //printf("with mid %d\n", data);

    console->memory[address] = (data&0xff);
}

uBYTE peek(SHORT address) {
    if (address < 0 || address > MEM_SIZE) {
        return 0;
    }
    return console->memory[address] & 0xff;
}

void pset(short x, short y, BYTE color) {
    short camx = (peek(0x5f28) | ((peek(0x5f29)<<8)));
    short camy = (peek(0x5f2a) | ((peek(0x5f2b)<<8)));
    x -= camx;
    y -= camy;
    /*if (x > 255 && x < 384) {
        printf("%d\n", camx);
        printf("uai\n");
    }*/
    /*if (camx !=0 ){
        printf("%d\n", camx);
        printf("%x %x %x\n", peek(0x5f28), (peek(0x5f29)<<8)|peek(0x5f28), camx);
    }*/
    uBYTE xx = MID(floor(x/2), 0, 64);
    uBYTE yy = MID(y, 0, 128);
    //printf("%d %d %x\n", x, xx, xx);

    SHORT address = ((xx%64)+(yy*64));
    BYTE col = console->gfx[address];

    BYTE clx = peek(0x5f20);
    BYTE cly = peek(0x5f21);
    BYTE clw = peek(0x5f22);
    BYTE clh = peek(0x5f23);

    //printf("%d\n", peek(0x5f00+(color&0xf)) & 0xf);
    BYTE pcol = peek(0x5f00+(color&0xf)) & 0xf;

    //printf("%d %d %d %d\n", clx, cly, clw, clh);

    if (x >= clx && y >= cly &&
        x <= clw && y <= clh) {
        if (x%2) {
            console->gfx[address] = ((col&0xf)) | (pcol<<4);
        } else {
            console->gfx[address] = (col&0xf0) | pcol;
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

void rect(short x0, short y0, short x1, short y1, BYTE color) {
    line(x0,y0,x1,y0,color);
    line(x1,y0+1,x1,y1,color);
    line(x1-1,y1,x0,y1,color);
    line(x0,y1-1,x0,y0,color);
}

void rectfill(short x0, short y0, short x1, short y1, BYTE color) {
    for (int yy=y0; yy <= (y1); yy++) {
        line(x0,yy,x1,yy,color);
    }
}

void printchar(BYTE c, short x, short y, BYTE color) {
    /*printf("%c%c%c\n%c%c%c\n%c%c%c\n%c%c%c\n%c%c%c\n", font[c][0], font[c][1], font[c][2],
                                                       font[c][3], font[c][4], font[c][5],
                                                       font[c][6], font[c][7], font[c][8],
                                                       font[c][9], font[c][10], font[c][11],
                                                       font[c][12], font[c][13], font[c][14]);*/
    //cursor(x, y);
    //BYTE cy = peek(0x5f21);
    //cursor(x, cy);
    //cx += 4;
    //BYTE cx = peek(0x5f20);
    if (console->state == CONSOLE) {
        rectfill(x-1,y-1,x+4,y+5,0);
        //cursor(cx+4,y);
    }
    for (short addr = 0; addr < 15; addr++) {
        short xx = addr % 3;
        short yy = floor(addr / 3);
        //printf("%d\n", font[c][addr]);
        if (font[c][addr] != 0) {
            //BYTE col = peek(0x5f00+color)&0xf;
            pset(xx+x, yy+y, font[c][addr]*color);
        }
    }
    //printf("%d\n", cx);
    //cursor(x, cy);
}

void print(const BYTE * text, short x, short y, BYTE color) {
    int j = 0;
    cursor(x, y);
    short xx = x;
    short yy = y;
    //printf("%d\n", x);
    for (const char * c = text; *c != '\0'; c++) {
        //printf("%c\n", *c);
        if (console->state == CONSOLE) {
            if (xx + (j*4) > 125) {
                xx -= 124;
                yy += 6;
            }
            //printf("%d\n", xx);
        }
        printchar(*c, xx + (j*4), yy, color);
        j++;
    }
    cursor(x, yy + 6);
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
    x = MID(x, -32768, 32767);
    y = MID(y, -32768, 32767);
    poke(0x5f28, MID(x & 0xff, 0, 0xff));
    poke(0x5f29, MID((x & 0xff00) >> 8, 0, 0xff));
    poke(0x5f2a, MID(y & 0xff, 0, 0xff));
    poke(0x5f2b, MID((y & 0xff00) >> 8, 0, 0xff));
    //printf("%x %x %x\n",(x & 0xff00)>>8,x&0xff, x);
}

void cursor(BYTE x, BYTE y) {
    poke(0x5f26, MID(x & 0xff, 0, 0xff));
    poke(0x5f27, MID(y & 0xff, 0, 0xff));
}

void color(BYTE color) {
    poke(0x5f25,color);
}

void clear(BYTE color) {
    SDL_RenderClear(console->render);
    for (int addr = 0; addr < 0x2000; addr++) {
        console->gfx[addr] = (color<<4) | color;
    }
    cursor(0,0);
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