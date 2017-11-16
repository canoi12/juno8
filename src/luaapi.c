#include <luaapi.h>
Juno * juno;

static Juno * lua_getJuno(lua_State * l) {
    lua_getglobal(l, "juno");
    Juno * juno = (Juno*)lua_touserdata(l, -1);
    lua_pop(l, 1);
    return juno;
}

int lua_init(lua_State * l) {
    //init();
    printf("%p\n", juno);
    return 0;
}

int lua_update(lua_State * l) {
    update(juno);
    return 0;
}

int lua_draw(lua_State * l) {
    draw(juno);
    return 0;
}

int lua_btn(lua_State * l) {
    BYTE key = luaL_checkinteger(l, 1);
    Juno * juno = lua_getJuno(l);
    lua_pushboolean(l, btn(juno,key));
    return 1;
}

int lua_btnp(lua_State * l) {
    BYTE key = luaL_checkinteger(l, 1);
    Juno * juno = lua_getJuno(l);
    lua_pushboolean(l, btnp(juno, key));
    return 1;
}

int lua_keydown(lua_State * l) {
    const char * key = luaL_checkstring(l, 1);
    Juno * juno = lua_getJuno(l);
    lua_pushboolean(l, keyDown(juno,key));
    return 1;
}

int lua_keypress(lua_State * l) {
    const char * key = luaL_checkstring(l, 1);
    Juno * juno = lua_getJuno(l);
    lua_pushboolean(l, keyPressed(juno,key));
    return 1;
}

int lua_pal(lua_State * l) {
    int n = lua_gettop(l);
    Juno * juno = lua_getJuno(l);
    if (n > 1) {
        BYTE col = luaL_checkinteger(l, 1);
        BYTE ncol = luaL_checkinteger(l, 2);
        BYTE offset = luaL_optinteger(l, 3, 0);
        pal(juno, col, ncol, offset);
    } else {
        pal(juno, 0,0,0);
        pal(juno, 0,0,1);
        palt(juno, 0,1);
        for (int i = 1; i < 16; i++) {
            pal(juno, i,i,0);
            pal(juno, i,i,1);
            palt(juno, i,0);

        }
    }
    return 0;
}

int lua_palt(lua_State * l) {
    int n = lua_gettop(l);
    Juno * juno = lua_getJuno(l);
    if (n > 1) {
        BYTE col = luaL_checkinteger(l, 1);
        BYTE transp = lua_toboolean(l, 2);
        palt(juno, col, transp);
    } else {
        palt(juno, 0,1);
        for (int i = 0; i < 16; i++) {
            palt(juno, i, 0);
        }
    }
    return 0;
}

int lua_input(lua_State * l) {
    char * aux = NULL;
    Juno * juno = lua_getJuno(l);
    lua_pushstring(l, input(juno));
    return 1;
}

int lua_peek(lua_State * l) {
    SHORT address = luaL_checkinteger(l, 1);
    Juno * juno = lua_getJuno(l);
    lua_pushnumber(l, peek(juno, address));
    return 1;
}

int lua_poke(lua_State * l) {
    SHORT address = luaL_checkinteger(l, 1);
    BYTE data = luaL_checkinteger(l, 2) & 0xff;
    Juno * juno = lua_getJuno(l);
    poke(juno, address, data);
    return 0;
}

int lua_pset(lua_State * l) {
    short x = luaL_checkinteger(l, 1);
    short y = luaL_checkinteger(l, 2);
    Juno * juno = lua_getJuno(l);
    BYTE color = luaL_optinteger(l, 3, juno->memory[0x5f25]);
    pset(juno, x,y,color);
    return 0;
}

int lua_pget(lua_State * l) {
    short x = luaL_checkinteger(l, 1);
    short y = luaL_checkinteger(l, 2);
    Juno * juno = lua_getJuno(l);
    lua_pushnumber(l, pget(juno, x, y));
    return 1;
}

int lua_spr(lua_State * l) {
    BYTE sn = luaL_checknumber(l, 1);
    short x = luaL_optinteger(l, 2, 0);
    short y = luaL_optinteger(l, 3, 0);
    BYTE flip_h = lua_toboolean(l, 4);
    BYTE flip_v = lua_toboolean(l, 5);
    Juno * juno = lua_getJuno(l);
    spr(juno, sn, x, y, flip_h, flip_v);
    return 0;
}

int lua_line(lua_State * l) {
    short x0 = luaL_checkinteger(l, 1);
    short y0 = luaL_checkinteger(l, 2);
    short x1 = luaL_checkinteger(l, 3);
    short y1 = luaL_checkinteger(l, 4);
    Juno * juno = lua_getJuno(l);
    BYTE color = luaL_optinteger(l, 5, juno->memory[0x5f25]);
    line(juno,x0,y0,x1,y1,color);
    return 0;
}

int lua_circ(lua_State * l) {
    short x = luaL_checkinteger(l, 1);
    short y = luaL_checkinteger(l, 2);
    short radius = luaL_checkinteger(l, 3);
    Juno * juno = lua_getJuno(l);
    BYTE color = luaL_optinteger(l, 4, juno->memory[0x5f25]);
    circ(juno,x, y, radius, color);
    return 0;
} 

int lua_circfill(lua_State * l) {
    short x = luaL_checkinteger(l, 1);
    short y = luaL_checkinteger(l, 2);
    short radius = luaL_checkinteger(l, 3);
    Juno * juno = lua_getJuno(l);
    BYTE color = luaL_optinteger(l, 4, juno->memory[0x5f25]);
    circfill(juno,x, y, radius, color);
    return 0;
} 

int lua_rect(lua_State * l) {
    short x = luaL_checkinteger(l, 1);
    short y = luaL_checkinteger(l, 2);
    short width = luaL_checkinteger(l, 3);
    short height = luaL_checkinteger(l, 4);
    Juno * juno = lua_getJuno(l);
    BYTE color = luaL_optinteger(l, 5, juno->memory[0x5f25]);
    rect(juno,x,y,width,height,color);
    return 0;
}

int lua_rectfill(lua_State * l) {
    short x = luaL_checkinteger(l, 1);
    short y = luaL_checkinteger(l, 2);
    short width = luaL_checkinteger(l, 3);
    short height = luaL_checkinteger(l, 4);
    Juno * juno = lua_getJuno(l);
    BYTE color = luaL_optinteger(l, 5, juno->memory[0x5f25]);
    rectfill(juno,x,y,width,height,color);
    return 0;
}

int lua_print(lua_State * l) {
    /*lua_getglobal(l, "tostring");
    if (lua_isboolean(l, 1)) {
        lua_pushboolean(l, lua_toboolean(l, 1));
    } else {
        lua_pushstring(l, luaL_checkstring(l, 1));
    }
    lua_pcall(l,1,1,0);*/
    Juno * juno = lua_getJuno(l);
    const BYTE * txt = luaL_checkstring(l, 1);
    //lua_pop(l, -1);
    //printf("%s\n", txt);
    short x = luaL_optinteger(l, 2, peek(juno,0x5f26));
    short y = luaL_optinteger(l, 3, peek(juno,0x5f27));
    BYTE color = luaL_optinteger(l, 4, peek(juno,0x5f25));
    /*if (n > 1) {
        short x = luaL_checkinteger(l, 2);
        short y = luaL_checkinteger(l, 3);
        print(txt, x, y, color);
    } else {
        print(txt, 0, 0, color);
    }*/
    //printf("%d\n", *bool_str);
    print(juno,txt, x, y, color);
    //free(bool_str);
    return 0;
}

int lua_clip(lua_State * l) {
    int n = lua_gettop(l);
    Juno * juno = lua_getJuno(l);
    if (n > 3) {
        BYTE x = luaL_checkinteger(l, 1);
        BYTE y = luaL_checkinteger(l, 2);
        BYTE w = luaL_checkinteger(l, 3);
        BYTE h = luaL_checkinteger(l, 4);

        clip(juno,x,y,w,h);
    } else {
        clip(juno,0,0,127,127);
    }
    return 0;
}

int lua_camera(lua_State * l) {
    int n = lua_gettop(l);
    Juno * juno = lua_getJuno(l);
    if (n > 1) {
        short x = luaL_checkinteger(l, 1);
        short y = luaL_checkinteger(l, 2);

        camera(juno,x,y);
    } else {
        camera(juno,0,0);
    }
    return 0;
}

int lua_cursor(lua_State * l) {
    Juno * juno = lua_getJuno(l);
    BYTE x = luaL_optinteger(l, 1, peek(juno,0x5f26));
    BYTE y = luaL_optinteger(l, 2, peek(juno,0x5f27));
    cursor(juno,x, y);
    return 0;
}

int lua_color(lua_State * l) {
    Juno * juno = lua_getJuno(l);
    BYTE col = luaL_checkinteger(l, 1) & 0xf;
    color(juno,col);
    return 0;
}

int lua_clear(lua_State * l) {
    Juno * juno = lua_getJuno(l);
    BYTE color = luaL_optinteger(l, 1, 0x0);
    clear(juno,color);
    return 0;
}

int jmp(lua_State * l) {
    //printf("testando\n");
    return 0;
}

static const struct luaL_Reg juno8[] = {
    {"init", lua_init},
    {"update", lua_update},
    {"draw", lua_draw},
    {"btn", lua_btn},
    {"btnp", lua_btnp},
    {"keydown", lua_keydown},
    {"keypress", lua_keypress},
    {"pal", lua_pal},
    {"palt", lua_palt},
    {"input", lua_input},
    {"peek", lua_peek},
    {"poke", lua_poke},
    {"pset", lua_pset},
    {"pget", lua_pget},
    {"spr", lua_spr},
    {"line", lua_line},
    {"circ", lua_circ},
    {"circfill", lua_circfill},
    {"rect", lua_rect},
    {"rectfill", lua_rectfill},
    {"print", lua_print},
    {"clip", lua_clip},
    {"camera", lua_camera},
    {"cursor", lua_cursor},
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