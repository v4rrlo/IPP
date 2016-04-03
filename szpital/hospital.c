#include <string.h> //strcmp, strcpy
#include <stdlib.h> //malloc
#include <stdio.h>  //printf

#include "parse.h"
#include "structure.h"
#include "global.h"

int main (int argc, char **argv){
    int debug = 0;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") != 0){
            printf("ERROR\n");
            return 1;
        }
        else
            debug = 1;
    }
    initStructure(debug);
    char input[MAXN];
    while (readLine(input) == 1){
        parsed_t *parsedInput = malloc(sizeof(parsed_t));
        fillParsedWithNull(parsedInput);
        //puts(input);
        parse(parsedInput, input);
        //puts(parsedInput->command);
        //printParsedObject(parsedInput);
        if (strcmp(parsedInput->command, ENTER) == 0){
            newDiseaseEnterDescription( parsedInput->name1,
                                        parsedInput->diseaseDescription);
        }
        else if (strcmp(parsedInput->command, COPY) == 0){
            newDiseaseCopyDescription(  parsedInput->name1,
                                        parsedInput->name2);
        }
        else if (strcmp(parsedInput->command, CHANGE) == 0){
            changeDescription(  parsedInput->name1,
                                parsedInput->diseaseIndex,
                                parsedInput->diseaseDescription);
        }
        else if (strcmp(parsedInput->command, PRINT) == 0){
            printDescription(   parsedInput->name1,
                                parsedInput->diseaseIndex);
        }
        else if (strcmp(parsedInput->command, DELETE) == 0){
            deletePatientData(  parsedInput->name1);
        }
        freeParsedObject(parsedInput);
        free(parsedInput);
    }
    tearDownStructure();
    return 0;
}
