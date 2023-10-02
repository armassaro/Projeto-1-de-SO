#include "headerLegal.h"

void printFirstAnimation(int yTerminal, int xTerminal) {

    init_pair(1, COLOR_BLUE, COLOR_BLUE);

    attron(COLOR_PAIR(1));

    for(int a = 0; a <= yTerminal; a++) { 

        for(int b = 0; b <= xTerminal; b++) {

            mvaddwstr(a, b, &fullBlock);
            refresh();

        }

        napms(50);

    }

    attroff(COLOR_PAIR(1));
    clear();
    bkgd(COLOR_PAIR(1));
    refresh();

}

void enterFolderName(int yTerminal, int xTerminal) {

    init_color(COLOR_GREY, 179, 174, 162);
    init_pair(2, COLOR_WHITE, COLOR_GREY);
    init_pair(3, COLOR_BLACK, COLOR_BLACK);
    
    char *archiveName = (char*) malloc(sizeof(char) * 35);

    shadowBox = newwin(yTerminal - 4, xTerminal - 10, 2, 6);
    wbkgd(shadowBox, COLOR_PAIR(3));
    wrefresh(shadowBox);

    mainWindow = newwin(yTerminal - 4, xTerminal - 10, 1, 4);
    getmaxyx(mainWindow, ymainWindow, xmainWindow);
    changeMainWindow("| Nome da pasta |");

    mvwprintw(mainWindow, (ymainWindow - 8) / 2, ((xmainWindow - strlen("Digite abaixo o nome do arquivo que você quer abrir:")) / 2), "Digite abaixo o nome do arquivo que você quer abrir:");
    wrefresh(mainWindow);

    do {

        inputInfo = newwin(3, xTerminal - 14, (yTerminal - 3) / 2, 6);
        getmaxyx(inputInfo, yinputInfo, xinputInfo);
        wbkgd(inputInfo, COLOR_PAIR(2));
        box(inputInfo, 0, 0);
        wrefresh(inputInfo);
        
        wmove(inputInfo, 1, 2);
        curs_set(TRUE);
        echo();
        nocbreak();
        
        wgetnstr(inputInfo, archiveName, 35);

        archive = fopen(archiveName, "rb");

        if(strcmp(archiveName, "parar") == 0) {

            werase(inputInfo);
            
            delwin(inputInfo);
            delwin(mainWindow);
            delwin(shadowBox);

            curs_set(FALSE);
            noecho();
            cbreak();
            
            shadowBox = newwin(3, (xTerminal - strlen("Parada forçada detectada!")) / 2 + 2, (yTerminal - 3) / 2 + 1, (xTerminal - (xTerminal - strlen("Parada forçada detectada!")) / 2 + 2) / 2 + 2);
            wbkgd(shadowBox, COLOR_PAIR(3));
            wrefresh(shadowBox);

            WINDOW *forcedStopWindow = newwin(3, (xTerminal - strlen("Parada forçada detectada!")) / 2 + 2, (yTerminal - 3) / 2, (xTerminal - (xTerminal - strlen("Parada forçada detectada!")) / 2 + 2) / 2);
            wbkgd(forcedStopWindow, COLOR_PAIR(2));
            mvwprintw(forcedStopWindow, 1, 4, "Parada forçada detectada!");
            box(forcedStopWindow, 0, 0);
            wrefresh(forcedStopWindow);
            napms(4000);

            endwin();
            exit(0);

        }

        delwin(inputInfo);

    }while(archive == NULL);

    free(archiveName);

    curs_set(FALSE);
    noecho();
    cbreak();

}

Process *readArchiveData() {
    
    Process *process = (Process*) malloc(sizeof(Process));
    auxIntReading = 0;

    while(feof(archive) == 0) {

        fread(process[auxIntReading].letter, sizeof(char), 1, archive);
        fread(process[auxIntReading].processDuration, sizeof(int), 1, archive);
        fread(process[auxIntReading].processTime, sizeof(int), 1, archive);

        process[auxIntReading].processStatus = ready;

        auxIntReading++;

        process = (Process*) realloc(process, sizeof(Process) * (auxIntReading));

    }

    auxIntReading--;

    return (Process*) realloc(process, sizeof(Process) * (auxIntReading));
 
}

void processManagement() {

    int asciiChar;
    Process *process;

    init_pair(4, COLOR_WHITE, COLOR_GREEN); //executing status
    init_pair(5, COLOR_WHITE, COLOR_YELLOW);  //ready status
    init_pair(6, COLOR_WHITE, COLOR_RED); //waiting status

    changeMainWindow("| Gerenciamento de processos SJF |");

    mvwprintw(mainWindow, ymainWindow - 2, 1, "Número de processo do programa: %i", programPID);
    wrefresh(mainWindow);

    process = readArchiveData();

    for(int a = 0; a < auxIntReading; a++) {
        
        asciiChar = 196 + '0';
        mvwprintw(mainWindow, 2 + a, 4, "[%c] %d Tempo restante: %i | ", process[a].letter, &asciiChar, process[a].processDuration);
        switch(process[a].processStatus) {

            case executing:
            attron(COLOR_PAIR(4));
            wprintw(mainWindow, "Executando");
            attroff(COLOR_PAIR(4));
            break;

            case ready:
            attron(COLOR_PAIR(5));
            wprintw(mainWindow, "Pronto");
            attroff(COLOR_PAIR(5));
            break;

            case waiting:
            attron(COLOR_PAIR(6));
            wprintw(mainWindow, "Em espera");
            attroff(COLOR_PAIR(6));
            break;
            
        }
        asciiChar = 192 + '0';
        mvwprintw(mainWindow, 3 + a, 4, " %d Tempo de entrada: %i", &asciiChar, process[a].processTime);
        
    }
    wrefresh(mainWindow);

}

void changeMainWindow(char *title) {

    attron(COLOR_PAIR(2));
    wclear(mainWindow);
    box(mainWindow, 0, 0);
    mvwprintw(mainWindow, 0, ((xmainWindow - strlen(title)) / 2), title);
    attroff(COLOR_PAIR(2));
    wbkgd(mainWindow, COLOR_PAIR(2));
    wrefresh(mainWindow);

}

int main() {

    initscr();  //inicia o modo ncurses
    cbreak();  //faz com que a entrada que o usuário enviar para o programa seja imediatamente lida, sem necessidade de dar enter
    noecho();  //desabilita a visualização da entrada do teclado do usuário no terminal
    curs_set(FALSE);  //desabilita a visualização do cursor dentro do temrinal
    start_color();
    setlocale(LC_ALL, "");
    
    programPID = getpid();

    getmaxyx(stdscr, yTerminal, xTerminal);

    init_pair(2, COLOR_BLACK, COLOR_BLUE);

    printFirstAnimation(yTerminal, xTerminal);

    enterFolderName(yTerminal, xTerminal);

    processManagement();

    getch();

    endwin(); //fecha o ncurses

    return 0;

}