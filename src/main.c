#include <juno8.h>
#include <luaapi.h>

char * state = "game";
char text[1024];

void rungame() {
    luaL_loadfile(console->l, "code.lua");

    lua_pcall(console->l, 0, 0, 0);

    lua_getglobal(console->l, "_init");
    lua_pcall(console->l, 0, 0, 0);

    printf("Iniciando o game loop\n");

    double start = SDL_GetTicks();

    int run = 0;

    while(!run) {
        update();
        switch(console->event.type) {
            case SDL_TEXTINPUT:
                strcat(text, console->event.text.text);
                break;

        }
        printf("%s\n", text);    
        draw();
        double current = SDL_GetTicks();
        if ((1000 / FPS) > current - start) {
            SDL_Delay((1000 / FPS) - (current-start));
        }


        run = console->keys[SDL_GetScancodeFromName("escape")];
        run = (console->event.type == SDL_QUIT);
        start = current;
    }
    //strcpy(state, "editor");
    printf("Saindo do jogo\n");
}

int main(int argc, char ** argv) {
    init();
    console->l = luaL_newstate();
    luaL_openlibs(console->l);
    luaopen_base(console->l);
    luaopen_juno8(console->l);

    while (console->event.type != SDL_QUIT) {
        /*if (!strcmp(state, "editor")) {
            SDL_PollEvent(&(console->event));
            if (console->keys[SDL_GetScancodeFromName("escape")]) {
                strcpy(state, "game");
            }
        } else {*/
            rungame();
        //}
    }

    SDL_DestroyWindow(console->window);
    SDL_DestroyRenderer(console->render);
    SDL_Quit();
    return 0;
}