#include <string.h> //isspace
#include <stdio.h> //getchar
#include <ctype.h> //for isspace

#include "global.h"

void catchWord(char *line, char *sub, int *counter) {
    int j = 0;  //catchWord is going to copy characters to sub starting from index j
    int i = *counter;
    while(isspace(UC line[i]))  //pushing through all the spaces in the string
        i++;                 //so it wouldn't return empty string or something
    while (!isspace(UC line[i]) && line[i] != '\n' && line[i] != '\0') {
        sub[j++] = line[i++];
    }
    sub[j] = '\0';
    *counter = i; // assigns value i to the old i
}

int readLine(char *input) {
    int c, i;
    for (i = 0; i < MAXN-1 && (c = getchar()) != EOF && c != '\n'; i++)
        input[i] = c;
    if (c == EOF)
        return 0;    // EOF, quitting the program
    input[i] = '\0';
    return 1; // continue the while loop in main
}
