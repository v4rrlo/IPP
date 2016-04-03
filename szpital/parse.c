#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdlib.h>

#include "global.h"
#include "parse.h"

typedef struct ParsedInput parsed_t; //description in parse.h
                                     //hospital.c needs to know the structure

void parseEnter(parsed_t *parsedResult, char *input,  int *k){
    char name1[MAXN];
    char diseaseDesc[MAXN];
    int l = *k;
    catchWord(input, name1, &l);
    *k = l;
    //rest of the string is a disease description, so we need to copy that
    strcpy(diseaseDesc, &input[l+1]);
    if (parsedResult->name1 == NULL){
        parsedResult->name1 = malloc((strlen(name1) + 1) * sizeof(char));
    }
    strcpy(parsedResult->name1, name1);
    if (parsedResult->diseaseDescription == NULL){
        parsedResult->diseaseDescription = malloc((strlen(diseaseDesc) + 1) * sizeof(char));
    }
    strcpy(parsedResult->diseaseDescription, diseaseDesc);
}

void parseChange(parsed_t *parsedResult, char *input, int *k){
    char name1[MAXN];
    char index[30];
    char diseaseDesc[MAXN];
    long indx;
    char *ptr;
    int l = *k;
    catchWord(input, name1, &l);
    *k = l;
    catchWord(input, index, &l);
    *k = l;
    strcpy(diseaseDesc, &input[l+1]);
    if (parsedResult->name1 == NULL)
        parsedResult->name1 = malloc((strlen(name1) + 1) * sizeof(char));
    strcpy(parsedResult->name1, name1);
    if (parsedResult->diseaseDescription == NULL)
        parsedResult->diseaseDescription = malloc((strlen(diseaseDesc) + 1) * sizeof(char));
    strcpy(parsedResult->diseaseDescription, diseaseDesc);
    indx = strtol(index, &ptr, 10);
    parsedResult->diseaseIndex = indx;
}

void parseCopy(parsed_t *parsedResult, char *input, int *k){
    char name1[MAXN];
    char name2[MAXN];
    int l = *k;
    catchWord(input, name1, &l);
    *k = l;
    catchWord(input, name2, &l);
    if (parsedResult->name1 == NULL)
        parsedResult->name1 = malloc((strlen(name1) + 1) * sizeof(char));
    strcpy(parsedResult->name1, name1);
    if (parsedResult->name2 == NULL)
        parsedResult->name2 = malloc((strlen(name2) + 1) * sizeof(char));
    strcpy(parsedResult->name2, name2);
}

void parsePrint(parsed_t *parsedResult, char *input, int *k){
    char name1[MAXN];
    char index[30];
    int l = *k;
    long indx;
    char *ptr;
    catchWord(input, name1, &l);
    *k = l;
    catchWord(input, index, &l);
    if (parsedResult->name1 == NULL)
        parsedResult->name1 = malloc((strlen(name1) + 1) * sizeof(char));
    strcpy(parsedResult->name1, name1);
    indx = strtol(index, &ptr, 10);
    parsedResult->diseaseIndex = indx;
}

void parseDelete(parsed_t *parsedResult, char *input, int *k){
    char name1[MAXN];
    int l = *k;
    catchWord(input, name1, &l);
    if (parsedResult->name1 == NULL)
        parsedResult->name1 = malloc((strlen(name1) + 1) * sizeof(char));
    strcpy(parsedResult->name1, name1);
}

void parse(parsed_t *parsedResult, char *input){
    int i = 0;
    char cmd[30];
    catchWord(input, cmd, &i);
    if(parsedResult->command == NULL)
        parsedResult->command = malloc((strlen(cmd) + 1) * sizeof(char));
    strcpy(parsedResult->command, cmd);
    if (strcmp(cmd, ENTER) == 0){ parseEnter(&(*parsedResult), input, &i); }
    else if (strcmp(cmd, COPY) == 0){ parseCopy(&(*parsedResult), input, &i); }
    else if (strcmp(cmd, CHANGE) == 0){ parseChange(&(*parsedResult), input, &i); }
    else if (strcmp(cmd, PRINT) == 0){ parsePrint(&(*parsedResult), input, &i); }
    else if (strcmp(cmd, DELETE) == 0){ parseDelete(&(*parsedResult), input, &i); }
}

void freeParsedObject(parsed_t *parsedResult){
    free(parsedResult->command);
    free(parsedResult->name1);
    free(parsedResult->name2);
    free(parsedResult->diseaseDescription);
}
void fillParsedWithNull(parsed_t *parsed){
    parsed->command = NULL;
    parsed->name1 = NULL;
    parsed->name2 = NULL;
    parsed->diseaseDescription = NULL;
    parsed->diseaseIndex = 0;
}

void printParsedObject(parsed_t *parsedResult){
    if (parsedResult->command != NULL)
        puts(parsedResult->command);
    if (parsedResult->name1 != NULL)
        puts(parsedResult->name1);
    if (parsedResult->name2 != NULL)
        puts(parsedResult->name2);
    if (parsedResult->diseaseDescription != NULL)
        puts(parsedResult->diseaseDescription);
    printf("diseaseIndex: %ld\n", parsedResult->diseaseIndex);
}
