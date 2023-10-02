#include <stdlib.h>
#include <stdio.h>

int main() {

    char auxChar;
    int auxInt;

    FILE *archiveBin = fopen("archiveBin.dat", "wb");
    if(archiveBin == NULL) {

        perror("erro arquivo bin\n");
        exit(1);

    }
    FILE *archiveTxt = fopen("archiveTxt.txt", "r");
    if(archiveTxt == NULL) {

        perror("erro de arquivo txt\n");
        exit(1);

    }
    while(feof(archiveTxt) == 0) {

        fscanf(archiveTxt, "%c ", &auxChar);
        fwrite(&auxChar, sizeof(char), 1, archiveBin);
        fscanf(archiveTxt, " %i", &auxInt);
        fwrite(&auxInt, sizeof(int), 1, archiveBin);
        fscanf(archiveTxt, " %i ", &auxInt);
        fwrite(&auxInt, sizeof(int), 1, archiveBin);
        
    }
}