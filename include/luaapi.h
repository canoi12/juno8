#ifndef LUALIB_H
#define LUALIB_H

#include <juno.h>
#include <api.h>


int lua_init(lua_State * l);
int lua_update(lua_State * l);
int lua_draw(lua_State * l);
int lua_btn(lua_State * l);
int lua_btnp(lua_State * l);
int lua_keydown(lua_State * l);
int lua_keypress(lua_State * l);
int lua_pal(lua_State * l);
int lua_palt(lua_State * l);
int lua_input(lua_State * l);
int lua_peek(lua_State * l);
int lua_poke(lua_State * l);
int lua_pset(lua_State * l);
int lua_pget(lua_State * l);
int lua_spr(lua_State * l);
int lua_line(lua_State * l);
int lua_circ(lua_State * l);
int lua_circfill(lua_State * l);
int lua_rect(lua_State * l);
int lua_rectfill(lua_State * l);
int lua_print(lua_State * l);
int lua_clip(lua_State * l);
int lua_camera(lua_State * l);
int lua_cursor(lua_State * l);
int lua_color(lua_State * l);
int lua_clear(lua_State * l);
int lua_flip(lua_State * l);

int luaopen_juno8(lua_State * l);
#endif // LUALIB_H