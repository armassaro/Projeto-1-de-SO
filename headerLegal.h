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

typedef enum {executing, ready, waiting, completed}ProcessStatus; //enum q representa os estados do processo

typedef struct {

    int waitingTime;  //tempo de espera do processo
    int responseTime;  //tempo de resposta do processo
    char letter;  //letra q representa o processo
    int arrivalTime;  //tempo de chegada
    int processTime;  //segunda variável de duração do processo
    int processDuration;  //duração do processo
    int processStatus;  //status do processo

}Process;

Process *process = NULL;

typedef struct {

    char letter;
    int arrivalTime;
    int processDuration;
    int processStatus;
    int id;

}SP;  //SP = ShortestProcess, armazena as info do processo mais curto definido pelo código

//functions
void printFirstAnimation(int yTerminal, int xTerminal);  //printa a primeira animação
void enterFolderName(int yTerminal, int xTerminal);  //tela q recebe o nome do arquivo
void changeMainWindow(char *title);  //limpa e troca o título da janela principal
void processManagement();  //possui toda a lógica do SJF preemptivo
void executionReport();  //retorna o relatório dos processos executados

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
int timeMax;

//color pairs info
// COLOR_PAIR(1) = blue text, blue screen = terminal colors
// COLOR_PAIR(2) = white text, grey screen = main window
// COLOR_PAIR(3) = black text, black screen = shadow box
// COLOR_PAIR(4) = white text, green screen = executing status
// COLOR_PAIR(5) = white text, yellow screen = ready status
// COLOR_PAIR(6) = white text, red screen = waiting status


#endif