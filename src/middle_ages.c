/** @file
    Main file of the game. Contains Implementation of the game loop.

    @author Miłosz Rzeźnikowski <mr371647@students.mimuw.edu.pl>
    @copyright Uniwersytet Warszawski
    @date 2016-05-15
    */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "parse.h"
#include "engine.h"


/**
    @name Global variables for turn handling.
    @{
    */
static int PLAYER_TURN = 0;   ///< Whose player's turn is now: 0 - turn of Player 1; 1 - turn of Player 2
static int PLAYER_1_INIT = 0; ///< Has Player 1 initialized?
static int PLAYER_2_INIT = 0; ///< Has Player 2 initialized?
int initSig = 0;              ///< Signal of init function, can't be local because it's used after calling init function.
command_t *new_command = NULL;///< Holds parsed command.
command_t *firstInit = NULL;  ///< Holds first INIT command.
/**
    @}
    */
/**
    Function that ends the program with exit code 42.
    Prints out "input error", ends the game (clearing structure) \
    and frees alloced command_t variables which contains firs parsed INIT \
    command and every next command.
    */
void SIGKILL(){
    fprintf(stderr, "input error\n");
    endGame();
    free(new_command);
    free(firstInit);
    exit(42);
}
/**
    Validtaing if number of arguments of the command is correct.
    If not, then ends program using SIGKILL() function.
    */
void validateAmountOfArguments(command_t *command, int n){
    for (int i = n; i < 7; i++){
        if (command->data[i] != 0){ //0 is default value when uninitialized
            SIGKILL(); //too many arguments
        }
    }
    //return nothing, everything is fine
}

/**
    Main loop of the game.
    Start's the game with initializing structure of players.
    While game has not resultet with a draw or victory of specific player, \
    reads commands and executes adequate functions of the structure.
    Whenever something goes wrong i.e construction of a command is invalid \
    it calls SIGKILL function.
    When the result of the game is clear then ends the game and frees \
    the structure and exists with exit code 0.
    */
int main(){
    startGame();
    firstInit = malloc(sizeof(command_t));
    while (gameStatus() < -1){
        free(new_command);
        new_command = parse_command();

        if (new_command == NULL)
            SIGKILL();
        else if (strcmp(new_command->name, "INIT") == 0){
            validateAmountOfArguments(new_command, 7);
            if(PLAYER_1_INIT + PLAYER_2_INIT == 0){ //no previous inits
                initSig = init(new_command->data[0],
                     new_command->data[1],
                     new_command->data[2],
                     new_command->data[3],
                     new_command->data[4],
                     new_command->data[5],
                     new_command->data[6]);
            }
            if (new_command->data[0] < 9
                    || new_command->data[1] < 1
                    || !(new_command->data[2] == 1 || new_command->data[2] == 2)
                    || new_command->data[3] < 1
                    || new_command->data[4] < 1
                    || new_command->data[5] < 1
                    || new_command->data[6] < 1
                    || initSig == 0){ //incorrect INIT command
                SIGKILL();
            }
            /* INIT command is valid so let's check who already INIT'ed */
            if (new_command->data[2] == 1 && PLAYER_1_INIT == 0){
                PLAYER_1_INIT = 1;
            }
            else if (new_command->data[2] == 2 && PLAYER_2_INIT == 0){
                PLAYER_2_INIT = 1;
            }
            else{
                SIGKILL();
            }
            if (PLAYER_1_INIT + PLAYER_2_INIT == 1){
                /* only one player init'ed so let's remember the first init
                 * so we can compare it to the second one later */
                firstInit->data[0] = new_command->data[0];
                firstInit->data[1] = new_command->data[1];
                firstInit->data[2] = new_command->data[2];
                firstInit->data[3] = new_command->data[3];
                firstInit->data[4] = new_command->data[4];
                firstInit->data[5] = new_command->data[5];
                firstInit->data[6] = new_command->data[6];
            }
            else if (PLAYER_1_INIT + PLAYER_2_INIT == 2){ //both players init'ed
                //checking if both INITs match each other
                if (firstInit->data[0] != new_command->data[0]
                        || firstInit->data[1] != new_command->data[1]
                        || firstInit->data[3] != new_command->data[3]
                        || firstInit->data[4] != new_command->data[4]
                        || firstInit->data[5] != new_command->data[5]
                        || firstInit->data[6] != new_command->data[6]){
                    SIGKILL();
                }
            }
            printTopLeft();
        }
        else if (PLAYER_1_INIT + PLAYER_2_INIT < 2) {
            //we got another command but both of players are not INIT'ed
            SIGKILL();
        }
        else{
            if (strcmp(new_command->name, "MOVE") == 0){
                validateAmountOfArguments(new_command, 4);
                int moveSig = move(PLAYER_TURN,
                     new_command->data[0],
                     new_command->data[1],
                     new_command->data[2],
                     new_command->data[3]);
                if (moveSig == 0){ //something went wrong
                    SIGKILL();
                }
                printTopLeft();
            }
            else if (strcmp(new_command->name, "PRODUCE_KNIGHT") == 0){
                validateAmountOfArguments(new_command, 4);
                int produceKnightSig = produceKnight(
                        PLAYER_TURN,
                        new_command->data[0],
                        new_command->data[1],
                        new_command->data[2],
                        new_command->data[3]);
                if (produceKnightSig == 0){
                    SIGKILL();
                }
                printTopLeft();
            }
            else if (strcmp(new_command->name, "PRODUCE_PEASANT") == 0){
                validateAmountOfArguments(new_command, 4);
                int producePeasantSig = producePeasant(
                        PLAYER_TURN,
                        new_command->data[0],
                        new_command->data[1],
                        new_command->data[2],
                        new_command->data[3]);
                if (producePeasantSig == 0){
                    SIGKILL();
                }
                printTopLeft();
            }
            else if (strcmp(new_command->name, "END_TURN") == 0){
                validateAmountOfArguments(new_command, 0);
                endTurn(PLAYER_TURN);
                PLAYER_TURN = (PLAYER_TURN + 1) % 2; // changing turns
            }
            else{
                SIGKILL();
            }
        }
    }
    if (new_command != NULL) free(new_command);
    if (firstInit != NULL) free(firstInit);

    endGame(); //prints who won
    exit(0);
}
