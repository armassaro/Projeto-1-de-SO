//define
#ifndef headerLegal
#define headerLegal
#define COLOR_GREY 8
#define _XOPEN_SOURCE_EXTENDED

//include
#include <stdio.h>
#include <stdlib.h>
#include <ncursesw/ncurses.h>
#include <locale.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

typedef enum {executing, ready, waiting, completed}ProcessStatus;

typedef struct {

    char letter;
    int arrivalTime;
    int processDuration;
    int processTime;
    int processStatus;

}Process;

//functions
void printFirstAnimation(int yTerminal, int xTerminal);
void enterFolderName(int yTerminal, int xTerminal);
void changeMainWindow(char *title);
void processManagement();

//variables
int auxIntReading;
int xTerminal, yTerminal;
wchar_t fullBlock = L'█';
WINDOW *mainWindow;
int ymainWindow, xmainWindow;
WINDOW *shadowBox;
WINDOW *inputInfo;
int yinputInfo, xinputInfo;
FILE *archive;

//color pairs info
// COLOR_PAIR(1) = blue text, blue screen = terminal colors
// COLOR_PAIR(2) = white text, grey screen = main window
// COLOR_PAIR(3) = black text, black screen = shadow box
// COLOR_PAIR(4) = white text, green screen = executing status
// COLOR_PAIR(5) = white text, yellow screen = ready status
// COLOR_PAIR(6) = white text, red screen = waiting status


#endif