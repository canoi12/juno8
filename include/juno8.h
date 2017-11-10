#ifndef JUNO8_H
#define JUNO8_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

//#include "junolua.h"

#define MEM_SIZE 32000
#define BYTE char
#define uBYTE unsigned char
#define SHORT unsigned short

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MID(v,mi,ma) (MAX((mi), MIN((v),(ma))))
#define FPS 30

static const Uint8 * keys;
#include <api.h>

typedef struct {
    uBYTE r;
    uBYTE g;
    uBYTE b;
} Color;

typedef struct {
    union {
        BYTE memory[MEM_SIZE];
        struct {
            BYTE sprites[0x2000];
            BYTE map[0x1000];
            BYTE flags[0x100];
            BYTE music[0x100];
            BYTE sfx[0x1100];
            BYTE user[0x1b00];
            BYTE persist[0x100];
            BYTE drawstate[0x40];
            BYTE hardstate[0x40];
            BYTE gpio[0x80];
            BYTE gfx[0x2000];
        };
    };
    Color palette[16];
    const Uint8 * keys;
    SDL_Window * window;
    SDL_Renderer * render;
    SDL_Event event;
    lua_State * l;
    BYTE scale;
} Console;
Console * console;

Color hexTorgb(int col);

void init();

void update();

void draw();

#endif // JUNO8_h