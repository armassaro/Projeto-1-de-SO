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

        archive = fopen(archiveName, "r");

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

void processManagement() {

    process = NULL;
    int timePassed = 0;
    int timeMax = 0;
    char *strAux = (char*) malloc(sizeof(char) * 30);

    changeMainWindow("| Gerenciamento de processos SJF |");

    process = (Process*) malloc(sizeof(Process));
    auxIntReading = 0;

    while(feof(archive) == 0) {

        fscanf(archive, "%c ", &process[auxIntReading].letter);
        fscanf(archive, "%i ", &process[auxIntReading].arrivalTime);
        fscanf(archive, "%i\n", &process[auxIntReading].processDuration);
        timeMax = timeMax + process[auxIntReading].processDuration;

        process[auxIntReading].processTime = process[auxIntReading].processDuration;
        process[auxIntReading].processStatus = ready;
        process[auxIntReading].waitingTime = 0;
        process[auxIntReading].responseTime = 0;

        auxIntReading++;

        process = (Process*) realloc(process, sizeof(Process) * (auxIntReading + 1));

    }

    SP shortestProcess;
    shortestProcess.processDuration = 99999;
    shortestProcess.id = -1;
    timePassed = 0;

    while(timePassed < timeMax) {

        SP shortestProcess;
        shortestProcess.processDuration = 99999;
        shortestProcess.id = -1;

        for(int a = 0; a < auxIntReading; a++) {

            if (process[a].processStatus != completed && process[a].arrivalTime <= timePassed && process[a].processDuration < shortestProcess.processDuration) {
                
                shortestProcess.processDuration = process[a].processDuration;
                shortestProcess.id = a;
                shortestProcess.letter = process[a].letter;
                shortestProcess.arrivalTime = process[a].arrivalTime;
            
            }
        
        }

        for(int a = 0; a < auxIntReading; a++) {
            
            if (a == shortestProcess.id) {
                
                process[a].processStatus = executing;
            
            } else {

                if (process[a].processStatus != completed) {
                    
                    process[a].processStatus = waiting;
                
                }

            } 

        }

        if (shortestProcess.id != -1) {

            process[shortestProcess.id].processDuration--;

            if(process[shortestProcess.id].processDuration <= 0) {

                process[shortestProcess.id].processStatus = completed;
                shortestProcess.processDuration = 99999;
            
            }
        
        }


         for (int a = 0; a < auxIntReading; a++) {
            
            if (process[a].processStatus == ready) {

                process[a].waitingTime++; // Incrementa o tempo de espera
            
            } else if (process[a].processStatus == waiting || process[a].processStatus == executing) {
            
                process[a].responseTime++; // Incrementa o tempo de resposta
            
            }

            mvwprintw(mainWindow, 2 + a, 4, "[%c] -> Tempo de entrada %i | Duração %i ", process[a].letter, process[a].arrivalTime, process[a].processDuration);

            switch(process[a].processStatus) {
                
                case executing:
                wprintw(mainWindow, "Executando");
                break;
                
                case ready:
                wprintw(mainWindow, "Pronto    ");
                break;
    
                case waiting:
                wprintw(mainWindow, "Em espera ");
                break;

                case completed:
                wprintw(mainWindow, "Completo  ");
                break;

            }
        
        }

        napms(1000); 
        sprintf(strAux, "Tempo decorrido: %i", timePassed);
        mvwprintw(mainWindow, 2, (xmainWindow / 2) - (strlen(strAux) / 2), strAux);
        
        wrefresh(mainWindow);
        timePassed++;

    }

    WINDOW *ShadowBox = newwin(3, (xTerminal - strlen("A execução dos processos foi concluída!")) / 2 + 2, (yTerminal - 3) / 2 + 1, (xTerminal - (xTerminal - strlen("A execução dos processos foi concluída!")) / 2 + 2) / 2 + 2);
    wbkgd(ShadowBox, COLOR_PAIR(3));
    wrefresh(ShadowBox);

    WINDOW *forcedStopWindow = newwin(3, (xTerminal - strlen("A execução dos processos foi concluída!")) / 2 + 2, (yTerminal - 3) / 2, (xTerminal - (xTerminal - strlen("A execução dos processos foi concluída!")) / 2 + 2) / 2);
    wbkgd(forcedStopWindow, COLOR_PAIR(2));
    mvwprintw(forcedStopWindow, 1, 4, "A execução dos processos foi concluída!");
    box(forcedStopWindow, 0, 0);
    wrefresh(forcedStopWindow);
    napms(4000);

    wclear(ShadowBox);
    wclear(forcedStopWindow);
    delwin(ShadowBox);
    delwin(forcedStopWindow);
    free(strAux);

}

void executionReport() {

    int auxInt = 0;

    changeMainWindow("| Relatório de execução |");
    wmove(mainWindow, 3, 4);
    wprintw(mainWindow, "Linha do tempo = ");
    wprintw(mainWindow, "[%i]", auxInt);

    for(int a = 0; a < auxIntReading; a++) {

        wprintw(mainWindow, "%c", process[a].letter);

        for(int b = 0; b < process[a].processTime; b++) {

            wprintw(mainWindow, "-");

        }

        auxInt = auxInt + process[a].processTime;

        wprintw(mainWindow, "[%i]", auxInt);

    }

    wmove(mainWindow, 5, strlen("Tempo de resposta = ") + 4);

    for(int a = 0; a < auxIntReading; a++) {

        wprintw(mainWindow, "%c ", process[a].letter);

    }
    
    mvwprintw(mainWindow, 6, 4, "Tempo de resposta   ");

    for(int a = 0; a < auxIntReading; a++) {

        wprintw(mainWindow, "%i ", process[a].responseTime);

    }

    mvwprintw(mainWindow, 7, 4, "Tempo de espera     ");

    for(int a = 0; a < auxIntReading; a++) {

        wprintw(mainWindow, "%i ", process[a].waitingTime);

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

    getmaxyx(stdscr, yTerminal, xTerminal);

    init_pair(2, COLOR_BLACK, COLOR_BLUE);

    printFirstAnimation(yTerminal, xTerminal);

    enterFolderName(yTerminal, xTerminal);

    processManagement();

    executionReport(process);

    getch();

    endwin(); //fecha o ncurses

    return 0;

}