#ifndef JUNO8_H
#define JUNO8_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>

//#include "junolua.h"

#define MEM_SIZE 32000
#define BYTE unsigned char
#define uBYTE unsigned char
#define SHORT unsigned short
#define true 1
#define false 0

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MID(v,mi,ma) (MAX((mi), MIN((v),(ma))))
#define FPS 30

static const Uint8 * keys;
#include <console.h>
//#include <console.h>
#include <api.h>

typedef struct Console Console;

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

typedef struct Juno Juno;

struct Juno {
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
    SDL_Texture * buffer;
    SDL_Event event;
    lua_State * l;
    State state;
    Console * console;
    //void * curState;
    void * teste;
    BYTE scale;
};

Color hexTorgb(int col);

Juno * init();

void update(Juno*);

void draw(Juno*);

int keyDown(Juno*, const char*);
int keyPressed(Juno*, const char*);

#endif // JUNO8_h