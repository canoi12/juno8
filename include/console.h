#ifndef CONSOLE_H
#define CONSOLE_H

#include <juno.h>

#define bool unsigned char

typedef struct Console Console;
typedef struct Juno Juno;

struct Console {
    //uBYTE * command;
    //int run;
    //char * last_char;
    Juno * juno;
    struct {
    	short x;
    	short y;
    	short delay;
    } cursor;

    struct {
    	short pos;
    	short start;

    	bool active;
    } scroll;
    BYTE * buffer;
    BYTE * colorBuffer;

    void(*render)(Console*);

};

//void initConsole();
//void updateConsole();
//void renderConsole();
//void clearConsole();

void initConsole(Console*, Juno*);

#endif // CONSOLE_H