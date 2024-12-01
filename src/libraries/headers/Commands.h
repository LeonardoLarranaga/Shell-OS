#ifndef COMMANDS_H
#define COMMANDS_H

#include "File Manager.h"
#include "Terminal.h"
#include "Memory.h"

void mycat(char**, int);
void type(char**, int);
void mycp(char**, int);
void myremove(char**, int);
void executeCommands(char**);
char** parseCommands(char*, const char*);

#endif