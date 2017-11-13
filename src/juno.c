#include <juno.h>

//int palette[2][16] = 
//_keys[] = {"left", "right", "up", "down", "x", "c"};
int palette[2][16] = {
    {
        0x000000,
        0x1d2b53,
        0x7e2553,
        0x008751,
        0xab5236,
        0x5f574f,
        0xc2c3c7,
        0xfff1e8,
        0xff004d,
        0xffa300,
        0xffec27,
        0x00e436,
        0x29adff,
        0x83769c,
        0xff77ab,
        0xffccaa
    },
    {
        0x140c1c,
        0x442434,
        0x30346d,
        0x4e4a4e,
        0x854c30,
        0x346524,
        0xd04648,
        0x757161,
        0x597dce,
        0xd27d2c,
        0x8595a1,
        0x6daa2c,
        0xd2aa99,
        0x6dc2ca,
        0xdad45e,
        0xdeeed6
    }

};

Color hexTorgb(int col) {
    //printf("%x %x %x %x\n", col, (col&0xff0000)>>16, (col&0xff00)>>8, (col&0xff));
    Color color = {(col&0xff0000)>>16, (col&0xff00)>>8, (col&0xff)};
    return color;
}

void init() {
    printf("Iniciando console...\n");
    initfont();
    SDL_Init(SDL_INIT_EVERYTHING);
    console = malloc(sizeof(Juno));
    printf("Memoria para console alocada\n");
    for (int addr = 0; addr < MEM_SIZE; addr++) {
        console->memory[addr] = 0x0;
    }
    console->state = CONSOLE;
    console->console = malloc(sizeof(Console));

    console->keys = SDL_GetKeyboardState(NULL);
    console->oldkeys = calloc(sizeof(Uint8),512);
    //console->memory[0x5f25] = 0x7;

    // CLIP
    clip(0,0,127,127);

    // COLOR
    color(0x7);

    // PAL and PALT
    pal(0,0,0);
    pal(0,0,1);
    palt(0,1);
    for (int i = 1; i < 16; i++) {
        pal(i,i,0);
        pal(i,i,1);
        palt(i,0);
    }

    // SDL
    console->window = SDL_CreateWindow("JUNO-8", SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       580, 540, SDL_WINDOW_SHOWN);
    console->render = SDL_CreateRenderer(console->window, -1, SDL_RENDERER_ACCELERATED);
    
    // SCALE
    console->scale = 4;

    // FILL PALETTE
    for (int addr = 0; addr < 16; addr++) {
        console->palette[addr] = hexTorgb(palette[1][addr]);
    }
    //console->palette[0x0] = hexTorgb(0x1d2b53);
    printf("Iniciado\n");
}

void update() {
    //printf("teste update\n");
    SDL_PollEvent(&(console->event));
    lua_getglobal(console->l, "_update");
    if(lua_pcall(console->l, 0, 0, 0)) {
        printf("Error in _update in pcall()");
    }
}

void draw() {
    SDL_RenderSetScale(console->render,console->scale,console->scale);
    SDL_SetRenderDrawColor(console->render, 0, 0, 0, 255);
    SDL_RenderClear(console->render);
    //clear(0);
    lua_getglobal(console->l, "_draw");
    if(lua_pcall(console->l, 0, 0, 0)) {
        printf("Error in _draw in pcall()\n");
    }

    //pset(1,0,8);
    //SDL_SetRenderDrawColor(console->render,255,0,0,255);
    flip();
    SDL_RenderPresent(console->render);
}

int keyDown(const char * key) {
    return console->keys[SDL_GetScancodeFromName(key)];
}

int keyPressed(const char * key) {
    int old = console->oldkeys[SDL_GetScancodeFromName(key)];
    int ret = keyDown(key) && !old;
    console->oldkeys[SDL_GetScancodeFromName(key)] = keyDown(key);
    return ret;
}