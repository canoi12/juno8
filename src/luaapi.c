#include <luaapi.h>

int lua_init(lua_State * l) {
    //init();
    printf("%p\n", console);
    return 0;
}

int lua_update(lua_State * l) {
    update();
    return 0;
}

int lua_draw(lua_State * l) {
    draw();
    return 0;
}

int lua_btn(lua_State * l) {
    BYTE key = luaL_checkinteger(l, 1);
    lua_pushboolean(l, btn(key));
    return 1;
}

int lua_peek(lua_State * l) {
    SHORT address = luaL_checkinteger(l, 1);
    lua_pushnumber(l, peek(address));
    return 1;
}

int lua_poke(lua_State * l) {
    SHORT address = luaL_checkinteger(l, 1);
    BYTE data = luaL_checkinteger(l, 2) & 0xff;
    poke(address, data);
    return 0;
}

int lua_pset(lua_State * l) {
    short x = luaL_checkinteger(l, 1);
    short y = luaL_checkinteger(l, 2);
    BYTE color = luaL_optinteger(l, 3, console->memory[0x5f25]);
    pset(x,y,color);
    return 0;
}

int lua_pget(lua_State * l) {
    short x = luaL_checkinteger(l, 1);
    short y = luaL_checkinteger(l, 2);
    lua_pushnumber(l, pget(x, y));
    return 1;
}

int lua_line(lua_State * l) {
    short x0 = luaL_checkinteger(l, 1);
    short y0 = luaL_checkinteger(l, 2);
    short x1 = luaL_checkinteger(l, 3);
    short y1 = luaL_checkinteger(l, 4);
    BYTE color = luaL_optinteger(l, 5, console->memory[0x5f25]);
    line(x0,y0,x1,y1,color);
    return 0;
}

int lua_circ(lua_State * l) {
    short x = luaL_checkinteger(l, 1);
    short y = luaL_checkinteger(l, 2);
    short radius = luaL_checkinteger(l, 3);
    BYTE color = luaL_optinteger(l, 4, console->memory[0x5f25]);
    circ(x, y, radius, color);
    return 0;
} 

int lua_circfill(lua_State * l) {
    short x = luaL_checkinteger(l, 1);
    short y = luaL_checkinteger(l, 2);
    short radius = luaL_checkinteger(l, 3);
    BYTE color = luaL_optinteger(l, 4, console->memory[0x5f25]);
    circfill(x, y, radius, color);
    return 0;
} 

int lua_rect(lua_State * l) {
    short x = luaL_checkinteger(l, 1);
    short y = luaL_checkinteger(l, 2);
    short width = luaL_checkinteger(l, 3);
    short height = luaL_checkinteger(l, 4);
    BYTE color = luaL_optinteger(l, 5, console->memory[0x5f25]);
    rect(x,y,width,height,color);
    return 0;
}

int lua_rectfill(lua_State * l) {
    short x = luaL_checkinteger(l, 1);
    short y = luaL_checkinteger(l, 2);
    short width = luaL_checkinteger(l, 3);
    short height = luaL_checkinteger(l, 4);
    BYTE color = luaL_optinteger(l, 5, console->memory[0x5f25]);
    rectfill(x,y,width,height,color);
    return 0;
}

int lua_clip(lua_State * l) {
    int n = lua_gettop(l);
    if (n > 3) {
        BYTE x = luaL_checkinteger(l, 1);
        BYTE y = luaL_checkinteger(l, 2);
        BYTE w = luaL_checkinteger(l, 3);
        BYTE h = luaL_checkinteger(l, 4);

        clip(x,y,w,h);
    } else {
        clip(0,0,127,127);
    }
    return 0;
}

int lua_camera(lua_State * l) {
    int n = lua_gettop(l);
    if (n > 1) {
        short x = luaL_checkinteger(l, 1);
        short y = luaL_checkinteger(l, 2);

        camera(x,y);
    } else {
        camera(0,0);
    }
    return 0;
}

int lua_color(lua_State * l) {
    BYTE col = luaL_checkinteger(l, 1) & 0xf;
    color(col);
    return 0;
}

int lua_clear(lua_State * l) {
    BYTE color = luaL_optinteger(l, 1, 0x0);
    clear(color);
    return 0;
}

int jmp(lua_State * l) {
    printf("testando\n");
    return 0;
}

static const struct luaL_Reg juno8[] = {
    {"init", lua_init},
    {"update", lua_update},
    {"draw", lua_draw},
    {"btn", lua_btn},
    {"peek", lua_peek},
    {"poke", lua_poke},
    {"pset", lua_pset},
    {"pget", lua_pget},
    {"line", lua_line},
    {"circ", lua_circ},
    {"circfill", lua_circfill},
    {"rect", lua_rect},
    {"rectfill", lua_rectfill},
    {"clip", lua_clip},
    {"camera", lua_camera},
    {"color", lua_color},
    {"clear", lua_clear},
    {"_init", jmp},
    {"_update", jmp},
    {"_draw", jmp},
    {NULL, NULL}
};

int luaopen_juno8(lua_State * l) {
    lua_pushglobaltable(l);
    luaL_setfuncs(l, juno8, 0);
    return 0;
}