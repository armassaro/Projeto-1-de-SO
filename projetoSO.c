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
    timeMax = 0;
    char *auxStr = (char*) malloc(sizeof(char) * 30);
    init_pair(4, COLOR_WHITE, COLOR_GREEN);
    init_pair(5, COLOR_WHITE, COLOR_YELLOW);
    init_pair(6, COLOR_WHITE, COLOR_RED);
    init_pair(7, COLOR_BLACK, COLOR_WHITE);

    changeMainWindow("| Gerenciamento de processos SJF |");

    WINDOW *timeWin = newwin(3, strlen("Tempo decorrido: 00") + 5, 2, (xmainWindow - strlen("Tempo decorrido: 00") + 4) / 2);
    wbkgd(timeWin, COLOR_PAIR(2));
    box(timeWin, 0, 0);
    wrefresh(timeWin);
    delwin(timeWin);

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

                if (process[a].processStatus != executing && process[a].processStatus != completed && timePassed >= process[a].arrivalTime) {
                    
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

            mvwprintw(mainWindow, 4 + a, 4, "[%c] -> Tempo de entrada %i\t|\tDuração %i\t|\t", process[a].letter, process[a].arrivalTime, process[a].processDuration);

            switch(process[a].processStatus) {
                
                case executing:
                wattron(mainWindow, COLOR_PAIR(4));
                wprintw(mainWindow, "Executando");
                wattroff(mainWindow, COLOR_PAIR(4));
                break;
                
                case ready:
                wattron(mainWindow, COLOR_PAIR(5));
                wprintw(mainWindow, "Pronto    ");
                wattroff(mainWindow, COLOR_PAIR(5));
                break;
    
                case waiting:
                wattron(mainWindow, COLOR_PAIR(6));
                wprintw(mainWindow, "Em espera ");
                wattroff(mainWindow, COLOR_PAIR(6));
                break;

                case completed:
                wattron(mainWindow, COLOR_PAIR(7));
                wprintw(mainWindow, "Completo  ");
                wattroff(mainWindow, COLOR_PAIR(7));
                break; 

            }
        
        }

        napms(1000);
        for(int a = 0; a < auxIntReading; a++) {

            if (process[a].processStatus == ready) {

                process[a].waitingTime++; // Incrementa o tempo de espera
                process[a].responseTime++;
            
            } else if (process[a].processStatus == waiting || process[a].processStatus == executing) {
            
                process[a].responseTime++; // Incrementa o tempo de resposta
                
            }

        }
        sprintf(auxStr, "Tempo decorrido: %i", timePassed);
        mvwprintw(mainWindow, 2, (xmainWindow / 2) - (strlen(auxStr) / 2), auxStr);
        
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
    free(auxStr);

}

void executionReport() {

    int auxInt = 0;

    changeMainWindow("| Relatório de execução |");
    wmove(mainWindow, 3, 4);
    wprintw(mainWindow, "Linha do tempo = ");

    for(int a = 0; a < auxIntReading; a++) {

        wprintw(mainWindow, "[%i]%c", auxInt, process[a].letter);

        for(int b = 0; b < 5; b++) {

            wprintw(mainWindow, "-");

        }

        auxInt = auxInt + process[a].processTime;

    }

    wprintw(mainWindow, "[%i]", auxInt);

    wmove(mainWindow, 5, 4);

    wprintw(mainWindow, "Processos lidos:    ");

    for(int a = 0; a < auxIntReading; a++) {

        wprintw(mainWindow, "%c\t", process[a].letter);

    }
    
    mvwprintw(mainWindow, 6, 4, "Tempo de resposta:  ");

    for(int a = 0; a < auxIntReading; a++) {

        wprintw(mainWindow, "%i\t", process[a].responseTime);

    }

    mvwprintw(mainWindow, 7, 4, "Tempo de espera:    ");
    
    for(int a = 0; a < auxIntReading; a++) {

        auxInt = auxInt + process[a].arrivalTime;
        wprintw(mainWindow, "%i\t", process[a].waitingTime);

    }

    wmove(mainWindow, 9, 4);
    wprintw(mainWindow, "Processos lidos:    ");
    for(int a = 0; a < auxIntReading; a++) {

        wprintw(mainWindow, "%c\t", process[a].letter);

    }

    wmove(mainWindow, 10,4);
    wprintw(mainWindow, "Tempos de chegada:  ");
    for(int a = 0; a < auxIntReading; a++) {

        wprintw(mainWindow, "%i\t", process[a].arrivalTime);

    }

    wmove(mainWindow, 11, 4);
    wprintw(mainWindow, "Tempos de duração:  ");
    for(int a = 0; a < auxIntReading; a++) {

        wprintw(mainWindow, "%i\t", process[a].processTime);

    }

    wmove(mainWindow, 13, 4);
    wprintw(mainWindow, "Tempo médio de espera: ");
    auxInt = 0;
    for(int a = 0; a < auxIntReading; a++) {

        auxInt = auxInt + process[a].waitingTime;

    }
    auxInt /= auxIntReading;

    wmove(mainWindow, 14, 4);
    wprintw(mainWindow, "Tempo médio de resposta: ");
    auxInt = 0;
    for(int a = 0; a < auxIntReading; a++) {

        auxInt = auxInt + process[a].waitingTime;

    }
    auxInt /= auxIntReading;
    wprintw(mainWindow, "%i", auxInt);

    mvwprintw(mainWindow, 15, 4, "Tempo de duração de execução dos processos: %i", timeMax);

    mvwprintw(mainWindow, ymainWindow - 2, (xmainWindow - strlen("Pressione qualquer tecla para sair")) / 2, "Pressione qualquer tecla para sair");

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

    free(process);

    endwin(); //fecha o ncurses

    return 0;

}