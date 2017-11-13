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
#define BYTE unsigned char
#define uBYTE unsigned char
#define SHORT unsigned short

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MID(v,mi,ma) (MAX((mi), MIN((v),(ma))))
#define FPS 30

static const Uint8 * keys;
#include <console.h>
//#include <console.h>
#include <api.h>

typedef struct console Console;

typedef enum {
    CONSOLE,
    CODE,
    SPRITE,
    MAP,
    SFX,
    MUSIC,
    GAME
} State;

typedef struct {
    uBYTE r;
    uBYTE g;
    uBYTE b;
} Color;

typedef struct {
    union {
        uBYTE memory[MEM_SIZE];
        struct {
            uBYTE sprites[0x2000];
            uBYTE map[0x1000];
            uBYTE flags[0x100];
            uBYTE music[0x100];
            uBYTE sfx[0x1100];
            uBYTE user[0x1b00];
            uBYTE persist[0x100];
            uBYTE drawstate[0x40];
            uBYTE hardstate[0x40];
            uBYTE gpio[0x80];
            uBYTE gfx[0x2000];
        };
    };
    Color palette[16];
    const Uint8 * keys;
    Uint8 * oldkeys;
    SDL_Window * window;
    SDL_Renderer * render;
    SDL_Event event;
    lua_State * l;
    State state;
    Console * console;
    void * teste;
    BYTE scale;
} Juno;
Juno * console;

Color hexTorgb(int col);

void init();

void update();

void draw();

int keyDown(const char * key);
int keyPressed(const char * key);

#endif // JUNO8_h