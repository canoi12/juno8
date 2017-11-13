#include <juno.h>
#include <luaapi.h>

char * state = "game";
char text[1024];

void rungame() {
    if (luaL_loadfile(console->l, "code.lua")) {
        printf("Error to load code.lua\n");
        exit(1);
    }

    if(lua_pcall(console->l, 0, 0, 0)) {
        printf("error in pcall()\n");
        exit(1);
    }

    lua_getglobal(console->l, "_init");
    if(lua_pcall(console->l, 0, 0, 0)) {
        printf("failed to load _init in pcall()\n");
        exit(1);
    }

    printf("Iniciando o game loop\n");

    double start = SDL_GetTicks();

    int run = 1;

    while(run) {
        update();
        /*switch(console->event.type) {
            case SDL_TEXTINPUT:
                strcat(text, console->event.text.text);
                break;

        }*/
        draw();
        if (keyPressed("escape")) {
            console->state = CONSOLE;
        }
        //printf("%s\n", text);    
        double current = SDL_GetTicks();
        if ((1000 / FPS) > current - start) {
            SDL_Delay((1000 / FPS) - (current-start));
        }


        //run = console->keys[SDL_GetScancodeFromName("escape")];
        run = (console->event.type != SDL_QUIT);
        run = run && console->state == GAME;
        start = current;
    }
    //strcpy(state, "editor");
    //lua_pop(console->l, -1);
    printf("Saindo do jogo\n");
}

void runconsole() {
    initConsole();
    console->console->run = 1;
    double start = SDL_GetTicks();
    printf("Entrando no console\n");
    SDL_StartTextInput();
    while(console->console->run) {
        updateConsole();
        renderConsole();
        double current = SDL_GetTicks();
        if ((1000 / FPS) > current - start) {
            SDL_Delay((1000 / FPS) - (current-start));
        }
        start = current;
        console->console->run = console->state == CONSOLE;
        console->console->run = console->console->run && console->event.type != SDL_QUIT;
        //printf("%d\n", console->console->run);
    }
    printf("Saindo do console\n");
    clearConsole();
}

int main(int argc, char ** argv) {
    init();
    console->l = luaL_newstate();
    luaL_openlibs(console->l);
    luaopen_base(console->l);
    luaopen_juno8(console->l);

    while (console->event.type != SDL_QUIT) {
        SDL_PollEvent(&(console->event));
        /*if (!strcmp(state, "editor")) {
            SDL_PollEvent(&(console->event));
            if (console->keys[SDL_GetScancodeFromName("escape")]) {
                strcpy(state, "game");
            }
        } else {*/
        //printf("%d\n", console->state);
        if (console->state == GAME) {
            printf("Entrando no estado game\n");
            rungame();
        } else if (console->state == CONSOLE) {
            printf("Entrando no estado console\n");
            runconsole();
        }
        //}
    }

    SDL_DestroyWindow(console->window);
    //SDL_DestroyRenderer(console->render);
    SDL_Quit();
    return 0;
}