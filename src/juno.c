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

Juno * init() {
    Juno * juno;
    printf("Iniciando console...\n");
    initfont();
    SDL_Init(SDL_INIT_EVERYTHING);
    juno = malloc(sizeof(Juno));
    printf("%p\n", juno);
    printf("Memoria para console alocada\n");
    juno->console = malloc(sizeof(Console));
    //if (!(juno->console->buffer)) {
    //    printf("Inicia essa bosta, po\n");
    //}
    initConsole(juno->console, juno);
    //printf("teste %p\n", juno->console->juno);
    //printf("render2 %p\n", juno->console->render);
    for (int addr = 0; addr < MEM_SIZE; addr++) {
        juno->memory[addr] = 0x0;
    }
    initSheet(juno);
    printf("%d\n", peek(juno,0x8));
    juno->state = GAME;
    //juno->console = malloc(sizeof(Console));

    juno->keys = SDL_GetKeyboardState(NULL);
    juno->oldkeys = calloc(sizeof(Uint8),512);

    juno->l = luaL_newstate();
    lua_pushlightuserdata(juno->l, juno);
    lua_setglobal(juno->l, "juno");
    //console->memory[0x5f25] = 0x7;

    // CLIP
    clip(juno,0,0,127,127);

    // COLOR
    color(juno,0x7);

    // PAL and PALT
    pal(juno,0,0,0);
    pal(juno,0,0,1);
    palt(juno,0,1);
    for (int i = 1; i < 16; i++) {
        pal(juno,i,i,0);
        pal(juno,i,i,1);
        palt(juno,i,0);
    }

    // SDL
    juno->window = SDL_CreateWindow("JUNO", SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       580, 540, SDL_WINDOW_SHOWN);
    juno->render = SDL_CreateRenderer(juno->window, -1, SDL_RENDERER_ACCELERATED);
    
    // SCALE
    juno->scale = 4;

    // FILL PALETTE
    for (int addr = 0; addr < 16; addr++) {
        juno->palette[addr] = hexTorgb(palette[1][addr]);
    }
    //console->palette[0x0] = hexTorgb(0x1d2b53);
    printf("Iniciado\n");
    return juno;
}

void update(Juno * juno) {
    //printf("teste update\n");
    SDL_PollEvent(&(juno->event));
    lua_getglobal(juno->l, "_update");
    if(lua_pcall(juno->l, 0, 0, 0)) {
        printf("Error in _update in pcall()");
    }
}

void draw(Juno * juno) {
    SDL_RenderSetScale(juno->render,juno->scale,juno->scale);
    SDL_SetRenderDrawColor(juno->render, 0, 0, 0, 255);
    SDL_RenderClear(juno->render);
    //clear(0);
    lua_getglobal(juno->l, "_draw");
    if(lua_pcall(juno->l, 0, 0, 0)) {
        printf("Error in _draw in pcall()\n");
    }

    //pset(1,0,8);
    //SDL_SetRenderDrawColor(console->render,255,0,0,255);
    flip(juno);
    SDL_RenderPresent(juno->render);
}

int keyDown(Juno * juno, const char * key) {
    return juno->keys[SDL_GetScancodeFromName(key)];
}

int keyPressed(Juno * juno, const char * key) {
    int old = juno->oldkeys[SDL_GetScancodeFromName(key)];
    int ret = keyDown(juno,key) && !old;
    juno->oldkeys[SDL_GetScancodeFromName(key)] = keyDown(juno,key);
    return ret;
}