#include <console.h>

/*const BYTE * logo = {
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
}*/

#define CONSOLE_BUFFER_WIDTH 32
#define CONSOLE_BUFFER_HEIGHT (128/6)

static void consolePrint(Console * console, const char * text, BYTE color) {
	const char * textPointer = text;
	const char * endText = textPointer + strlen(text);
	//	printf("%x %x\n",textPointer, endText);
	//printf("Eita\n");

	while (textPointer != endText) {
		char symbol = *textPointer++;

		if (symbol == '\n') {
			console->cursor.x = 0;
			console->cursor.y++;
		} else {
			short offset = console->cursor.x + console->cursor.y * CONSOLE_BUFFER_WIDTH;
			*(console->buffer + offset) = symbol;
			*(console->colorBuffer + offset) = color;

			console->cursor.x++;
			if (console->cursor.x >= CONSOLE_BUFFER_WIDTH) {
				console->cursor.x = 0;
				console->cursor.y++;
			}
		}
	}
	/*while (textPointer != endText) {
		char symbol = *textPointer++;
		printf("%c\n", symbol);

		if (symbol == '\n') {
			console->cursor.x = 0;
			console->cursor.y++;
		} else {
			short offset = console->cursor.x + console->cursor.y * CONSOLE_BUFFER_WIDTH;
			//printf("%d\n", offset);
			*(console->buffer + offset) = symbol;
			*(console->colorBuffer + offset) = color;

			console->cursor.x++;

			if (console->cursor.x >= CONSOLE_BUFFER_WIDTH) {
				console->cursor.x = 0;
				console->cursor.y++;
			}
		}
	}*/
}

static void printBack(Console * console, const char * text) {
	consolePrint(console, text, 0x5);
}

static void printFront(Console * console, const char * text) {
	consolePrint(console, text, 0x7);
}

static void printError(Console * console, const char * text) {
	consolePrint(console, text, 0x8);
}

static void printLine(Console * console) {
	consolePrint(console, "\n", 0);
}

static void drawConsoleText(Console * console) {
	clear(console->juno, 0);
	char * pointer = console->buffer + console->scroll.pos * CONSOLE_BUFFER_WIDTH;
	BYTE * colorPointer = console->colorBuffer + console->scroll.pos * CONSOLE_BUFFER_WIDTH;

	const char * end = console->buffer + (CONSOLE_BUFFER_WIDTH*CONSOLE_BUFFER_HEIGHT);
	short x = 0;
	short y = 0;

	while(pointer < end) {
		char symbol = *pointer++;
		BYTE color = *colorPointer++;

		if (symbol)
			printchar(console->juno,symbol, x * 4, y * 5, color);

		if (++x == CONSOLE_BUFFER_WIDTH) {
			y++;
			x = 0;
		}
	}
}

void renderConsole(Console * console) {
	clear(console->juno, 0);
	//printf("teste\n");

	//drawConsoleText(console);
	//print(console->juno, "olar", 0, 0, 7);
	//print(console->juno, "tudo bain?", 0, 6, 8);
	//printf("eita\n");
	drawConsoleText(console);
}

void initConsole(Console * console, Juno * juno) {
	console->buffer = SDL_malloc(CONSOLE_BUFFER_WIDTH*CONSOLE_BUFFER_HEIGHT);
	memset(console->buffer, 0, CONSOLE_BUFFER_WIDTH*CONSOLE_BUFFER_HEIGHT);
	console->colorBuffer = SDL_malloc(CONSOLE_BUFFER_WIDTH*CONSOLE_BUFFER_HEIGHT);
	memset(console->colorBuffer, 0, CONSOLE_BUFFER_WIDTH*CONSOLE_BUFFER_HEIGHT);

	*console = (Console) {
		.juno = juno,
		.cursor = {.x=0, .y=0, .delay=0},
		.scroll = {.pos=0,.start=0,.active=false},
		.buffer = console->buffer,
		.colorBuffer = console->colorBuffer,
		.render = renderConsole,
	};

	//printf("render %p\n", console->render);

	//printFront(console, ">");
	printFront(console, ">");
	printBack(console, "kkk eae men.");
}