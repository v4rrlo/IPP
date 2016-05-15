/** @file
    Implementation of parser.
    Parser reads line, checks if it's valid and returns structure containing
    name of command and it's integer arguemnts.

    @author Miłosz Rzeźnikowski <mr371647@students.mimuw.edu.pl>
    @copyright Uniwersytet Warszawski
    @date 2016-05-15
    */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXN 100 ///< Maximum length of a command.

/**
    Structure containing parsed arguments.
    */
typedef struct DefCommand {
    char name[16];  ///< Contains name of command.
    int data[7];    ///< Conatins parsed integer aguments of a command.
} command_t;

/**
    @name Subsidiary functions
    @{
    */
/**
    Reads line.
    @param [in] input Pointer to an array where we store input
    @return 0 if met EOF character
    @return 1 if everything is alright
    */
int readLine(char *input) {
    int c, i;
    for (i = 0; i < MAXN-1 && (c = getchar()) != EOF && c != '\n'; i++)
        input[i] = (char)c;
    if (c == EOF)
        return 0;
    input[i] = '\n';
    input[i+1] = '\0';
    return 1;
}
/**
    Checks line if it's valid with specified restrictions.
    @param [in] input Input to be checked.
    @return 0 if Input is invalid.
    @return 1 if Input is valid.
    */
int checkLine(char *input){
    int spaceCount = 0;
    for(int i = 0; i < MAXN-1 && input[i] != '\0'; i++){
        if (input[i+1] == '\0' && input[i] != '\n') // '\n' is not last character
            return 0;
        if (input[i+1] == '\n' && input[i] == ' ') //ends with space?
            return 0;
        if (isspace(input[i]) && input[i] != ' ' && input[i] != '\n') //different whitespace than space
            return 0;
        if (spaceCount > 1) //more than one consecutive space
            return 0;
        if (input[i] == ' ')
            spaceCount++;
        else
            spaceCount = 0;
    }
    return 1; //input is considered as 'correct'
}
/**@}*/
/** @name Parsing function
    @{
        At first fills name of command with NULL characters \
        and initializes integer arguments with 0's.
        Then it is reading input and checks if it is valid.
        Next thing that happens is reading command_name and then \
        reads numeric arguements and transforms them to integers.
        @return NULL when input is invalid
        @return parsed Structure with parsed arguments.
    @}    
    */
command_t* parse_command() {
    command_t *parsed = malloc(sizeof(command_t));
    for(int i = 0; i < 16; i++)
        parsed->name[i] = '\0';
    for(int i = 0; i < 7; i++)
        parsed->data[i] = 0;

    char input[100], intBuffer[100], *ptr;
    readLine(input);
    if (checkLine(input) == 0){ //returning NULL so we can interpret this as an 'input error'
        free(parsed);
        return NULL;
    }   //further checking for 'input errors' is taking place in main loop
    int spaceCount = 0, i = 0, j = 0;
    while (input[i] != '\n' && input[i] != '\0'){
        if (isspace(input[i])){
            spaceCount++;
            i++;
            continue;
        }
        if (spaceCount == 0){
            while (!isspace(input[i]) && input[i] != '\0' && input[i] != '\n'){
                parsed->name[i] = input[i];
                i++;
            }
        }
        else {
            while (!isspace(input[i]) && input[i] != '\0' && input[i] != '\n'){
                intBuffer[j++] = input[i++];
            }
            long tmp = strtol(intBuffer, &ptr, 10);
            if (tmp > 2147483647){
                return NULL;
            }
            parsed->data[spaceCount-1] = (int)tmp;
        }
        j = 0;
        for(int x = 0; x < 100; x++){
            intBuffer[x] = '\0'; //clearing out the buffer
        }
    }
    return parsed;
}