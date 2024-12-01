#ifndef COMMANDS_H
#define COMMANDS_H

void mycat(char**, int);
void type(char**, int);
void mycp(char**, int);
void myremove(char**, int);
void mkprocess(char**, int);
void lsprocesses();
void fcfs();
void sjf();
void roundRobin(char**, int);
void executeCommands(char**);
char** parseCommands(char*, const char*);

#endif