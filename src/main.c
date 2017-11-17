#include <juno.h>
#include <luaapi.h>

char * state = "game";
char text[1024];

static void rungame(Juno * juno) {
    if (luaL_loadfile(juno->l, "code.lua")) {
        printf("Error to load code.lua\n");
        exit(1);
    }

    if(lua_pcall(juno->l, 0, 0, 0)) {
        printf("error in pcall()\n");
        exit(1);
    }

    lua_getglobal(juno->l, "_init");
    if(lua_pcall(juno->l, 0, 0, 0)) {
        printf("failed to load _init in pcall()\n");
        exit(1);
    }

    printf("Iniciando o game loop\n");

    double start = SDL_GetTicks();

    int run = 1;

    while(run) {
        update(juno);
        /*switch(console->event.type) {
            case SDL_TEXTINPUT:
                strcat(text, console->event.text.text);
                break;

        }*/
        int width, height;

        SDL_GetWindowSize(juno->window, &width, &height);

        int size = floor(MIN(width/128, height/128));
        juno->scale = size;
        draw(juno);
        if (keyPressed(juno,"escape")) {
            juno->state = CONSOLE;
        }
        //printf("%s\n", text);    
        double current = SDL_GetTicks();
        if ((1000 / FPS) > current - start) {
            SDL_Delay((1000 / FPS) - (current-start));
        }


        //run = console->keys[SDL_GetScancodeFromName("escape")];
        run = (juno->event.type != SDL_QUIT);
        run = run && juno->state == GAME;
        start = current;
    }
    //strcpy(state, "editor");
    //lua_pop(console->l, -1);
    printf("Saindo do jogo\n");
}

static void runconsole(Juno * juno) {
    /*initConsole();
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
    clearConsole();*/
    //renderConsole(console->console);
    //printf("Render console!\n");
    juno->console->render(juno->console);
    if (keyPressed(juno, "escape")) {
        juno->state = GAME;
    }
    //console->state = GAME;
}

int main(int argc, char ** argv) {
    Juno * juno = init();
    printf("render3 %d\n", juno->console->cursor.x);
    //juno->l = luaL_newstate();
    //printf("eita\n");
    printf("JUNO %p\n", juno);
    printf("CONSOLE %p\n", juno->console->juno);
    printf("LUA %p\n", juno->l);
    luaL_openlibs(juno->l);
    luaopen_base(juno->l);
    luaopen_juno8(juno->l);

    printf("Iniciando loop\n");
    while (juno->event.type != SDL_QUIT) {
        SDL_PollEvent(&(juno->event));
        //SDL_RenderSetScale(juno->render, juno->scale, juno->scale);
        SDL_SetRenderDrawColor(juno->render, 0, 0, 0, 255);
        SDL_RenderClear(juno->render);

        int width, height;

        SDL_GetWindowSize(juno->window, &width, &height);

        int size = floor(MIN(width/128, height/128));
        juno->scale = size;
        /*if (!strcmp(state, "editor")) {
            SDL_PollEvent(&(console->event));
            if (console->keys[SDL_GetScancodeFromName("escape")]) {
                strcpy(state, "game");
            }
        } else {*/
        //printf("%d\n", console->state);
        if (juno->state == GAME) {
            printf("Entrando no estado game\n");
            rungame(juno);
        } else if (juno->state == CONSOLE) {
            //printf("Entrando no estado console\n");
            runconsole(juno);
        }
        flip(juno);
        //flip(juno);
        //}
    }
    free(juno->console);
    free(juno);
    SDL_DestroyWindow(juno->window);
    //SDL_DestroyRenderer(console->render);
    SDL_Quit();
    return 0;
}