#ifndef CONSOLE_H
#define CONSOLE_H

#include <juno.h>

typedef struct console {
    uBYTE * command;
    int run;
    char * last_char;
} Console;

void initConsole();
void updateConsole();
void renderConsole();
void clearConsole();

#endif // CONSOLE_H