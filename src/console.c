#include <console.h>

const BYTE * logo = {
	"000000000000000000000000000000000000000"\
	"000000000000000000000000000000000000000"\
	"000000000000000000000000000000000000000"\
	"000000000000000000000000000000000000000"\
	"000000000000000000000000000000000000000"\
};


void initConsole() {
	//print("JUNO 8", 0, 0, 7);
	//console->console->command = calloc(sizeof(BYTE), 32);
	//console->console->command[0] = '>';
	//console->console->run = 1;
	//console->console->last_char = calloc(sizeof(char), 5);
	luaL_loadfile(console->l, "console.lua");
	
	lua_pcall(console->l, 0, 0, 0);

	lua_getglobal(console->l, "_init");
	lua_pcall(console->l, 0, 0, 0);

	SDL_StartTextInput();
}

void updateConsole() {
	while(SDL_PollEvent(&(console->event))) {
		//input(console->console->command);
		lua_getglobal(console->l, "_input");
		lua_pcall(console->l, 0,0,0);

	}
	if (keyPressed("escape")) {
		console->state = GAME;
		//printf("okeh\n");
	}
	lua_getglobal(console->l, "_update");
	lua_pcall(console->l, 0, 0, 0);
}

void renderConsole() {
	//clear();
	SDL_RenderSetScale(console->render,console->scale,console->scale);
    SDL_SetRenderDrawColor(console->render, 0, 0, 0, 255);
    SDL_RenderClear(console->render);
    //cursor(0,0);
    //clear(0);
    //short x = peek(0x5f26);
    //short y = peek(0x5f27);
    //rectfill(x-1,y-1,x+6,y+6,6);
    //print(">",1,y,7);
	//print(console->console->command, 6, y, 7);
	lua_getglobal(console->l, "_draw");
	lua_pcall(console->l, 0, 0, 0);

	//rectfill(x-1, y-1, x+4, y+6, 0);
	flip();
	SDL_RenderPresent(console->render);
}

void clearConsole() {
	//free(console->console->command);
	//lua_pop(console->l, 1);
}