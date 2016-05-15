/** @file
    Implemetation of game engine.
    
    @author Miłosz Rzeźnikowski <mr371647@students.mimuw.edu.pl>
    @copyright Uniwersytet Warszawski
    @date 2016-05-15
    */

#include <stdio.h>
#include <stdlib.h>

/**
    Structure representing a node of list of pawns. Pawn has one of specific roles: King, Peasant or Knight.
    */
typedef struct Pawn{ //Pawn itself is kind of a Node, it has pointer to next pawn as in a list
    int posX;       ///< Column number of the pawn.
    int posY;       ///< Row number of the pawn. 
    char role;          ///< Role of the pawn. Is one of: 'K', 'C', 'R' or 'k', 'c', 'r'
    int lastMove;       ///< Turn number when moved last.
    struct Pawn *next;  ///< Pointer to the next pawn.
}pawn_t;

/**
    Structure representing a player.
    */
typedef struct Player{
    int turnNum;    ///< Number of the current turn.
    int status;     ///< Status of his turn: 1 - ended turn; 0 - still taking a turn.
    int hasLivingKing; ///< Status of the king: 1 - king still lives; 0 - king's dead.
    pawn_t *pawns;  ///< List of player's pawns
    pawn_t *lastPawn; ///< Pointer (to the last element of the list) which saves a lot of time when adding new pawn to the list
}player_t;

/**
    @name Global variables.
    @{
    */
static int MAX_TURNS_PER_PLAYER = 999999; ///< Limit of turns for each player. Value changes with first init.
static int GLOBAL_TURN = 0; ///< Counter of global turns.
static int BOARD_SIZE = 0;  ///< Size of the board.
static char BOARD[11][11];  ///< TopLeft fragment of the board with pawns marked on it.

static player_t *PLAYER_1;  ///< First player.
static player_t *PLAYER_2;  ///< Second player.
/**
    @}
    */

/**
    @name Board functions
    @{
    */
/**
    Printf top left fragment of the board.
    */
void printTopLeft() {
    int n = 10;
    if (BOARD_SIZE < n){
        n = BOARD_SIZE;
    }
    for(int i = 1; i <= n; i++){
        for(int j = 1; j <= n; j++){
            printf("%c", BOARD[j][i]);
        }
        printf("\n");
    }
    printf("\n");
}

/**
    Updates the board.
    Checks if given coordinates takes place in top left fragment. If so then \
    updates the board with actual situation on the board.
    First, it clears the board.
    Then every pawn with coordinates inside of fragment's area is marked on the board.
    */
void updateBoard(int x1, int y1, int x2, int y2){
    if (!((x1 > 0 && y1 > 0) || (x2 < 11 && y2 < 11))){
        return;
    }
    for (int i = 1; i < 11; i++){
        for (int j = 1; j < 11; j++){
            BOARD[i][j] = '.';
        }
    }
    pawn_t *current = PLAYER_1->pawns;
    while (current != NULL){
        if (current->posX <= 10 && current->posY <= 10){
            BOARD[current->posX][current->posY] = current->role;
        }
        current = current->next;
    }
    current = PLAYER_2->pawns;
    while (current != NULL){
        if (current->posX <= 10 && current->posY <= 10){
            BOARD[current->posX][current->posY] = current->role;
        }
        current = current->next;
    }
}

/**
    Validates if given coordinates are correct, meaning if the not exceed borders of the board.
    @return 0 If coordinates are invalid.
    @return 1 In the other case.
    */
int validateCoordinates(int x1, int y1, int x2, int y2){
    if (abs(x1 - x2) > 1 || abs(y1 - y2) > 1    //wrong coordinates
        || x2 > BOARD_SIZE || y2 > BOARD_SIZE
        || x1 > BOARD_SIZE || y1 > BOARD_SIZE
        || x1 < 1 || x2 < 1 || y1 < 1 || y2 < 1){
        return 0; //incorrect action
    }
    return 1;
}
/** 
    @}
    */

/**
    @name Pawn functions.
    @{
    */
/**
    Finds pawn with given coordinates.
    @param[in] playerNum Whose player is that pawn.
    @param[in] x Column number of the pawn.
    @param[in] y Row number of the pawn.
    @return NULL If pawn with given coordinates is not found.
    @return pawn_t If found.
    */
pawn_t* findPawn(int playerNum, int x, int y){
    pawn_t *current;
    if (playerNum == 0){
        current = PLAYER_1->pawns;
        while (current != NULL) {
            if ((current)->posX == x && (current)->posY == y){
                return current;
            }
            current = (current)->next;
        }
        return NULL;
    }
    else if (playerNum == 1){
        current = PLAYER_2->pawns;
        while (current != NULL) {
            if ((current)->posX == x && (current)->posY == y) {
                return current;
            }
            current = (current)->next;
        }
    }
    return NULL;
}
/**
    Adds new pawn to player's list of pawns.
    @param[in] player Player to which we add a pawn.
    @param[in] role Role of added pawn.
    @param[in] x Column number of added pawn.
    @param[in] y Row number of added pawn.
    */
void addPawn(player_t **player, char role, int x, int y){
    pawn_t **current = &(*player)->pawns;
    pawn_t *new = malloc(sizeof(pawn_t));
    new->next = NULL;
    new->posX = x;
    new->posY = y;
    new->lastMove = (*player)->turnNum - 1; //it still can make a move in a turn its spawned
    new->role = role;
    if (*current == NULL){
        *current = new;
        (*player)->lastPawn = (*current);
        return;
    }
    (*player)->lastPawn->next = new;
    (*player)->lastPawn = (*player)->lastPawn->next;
}

/**
    Removes pawn from player's list.
    @param[in] player Player from which we remove a pawn.
    @param[in] pawn Pawn which we remove.
    */
void removePawnFromPlayer(player_t **player, pawn_t **pawn){
    pawn_t *current = (*player)->pawns;
    pawn_t *previous = NULL;
    while (current->posX != (*pawn)->posX || current->posY != (*pawn)->posY){
        previous = current;
        current = current->next;
    }
    if (previous){
        if (current->next){
            previous->next = current->next;
        }
        else{
            previous->next = NULL;
            (*player)->lastPawn = previous;
        }
        free(current);
    }
    else{
        if(current->next){
            (*player)->pawns = current->next;
        }
        else{
            (*player)->pawns = NULL;
            (*player)->lastPawn = (*player)->pawns;
        }
        free(current);
    }
}
/**
    Removes pawn.
    Calls removePawnFromPlayer after recognizing whose player's is that pawn.
    @param[in] pawn Pawn to remove.
    */
void removePawn(pawn_t **pawn){
    if ((*pawn)->role <= 'Z'){ //pawn of first player
        if ((*pawn)->role == 'K'){
            PLAYER_1->hasLivingKing = 0;
        }
        removePawnFromPlayer(&PLAYER_1, pawn);
    }
    else{
        if ((*pawn)->role == 'k'){
            PLAYER_2->hasLivingKing = 0;
        }
        removePawnFromPlayer(&PLAYER_2, pawn);
    }
}
/**
    Moves pawn.
    @param[in] pawn Pawn to move.
    @param[in] x Column number of destination point.
    @param[in] y Row number of destination point.
    */
void movePawn(pawn_t **pawn, int x, int y){
    (*pawn)->posX = x;
    (*pawn)->posY = y;
    (*pawn)->lastMove = GLOBAL_TURN;
}
/**
    Tears down list of pawns.
    @param[in] pawns Pointer to the head of list of pawns.
    */
void tearDownPawns(pawn_t **pawns){
    pawn_t *current = *pawns;
    pawn_t *next;
    while(current != NULL){
        next = current->next;
        free(current);
        current = next;
    }
    *pawns = NULL;
}
/**
    @}
    */

/**
    @name Game functions.
    @{
    */
/**
    Sets statuses of players to 'new turn' status and updates their actual turn number.
    @param[in] turnNum Number of new turn.
    */
void setStatusNewTurn(int turnNum){
    PLAYER_1->status = 0;
    PLAYER_2->status = 0;
    PLAYER_1->turnNum = turnNum;
    PLAYER_2->turnNum = turnNum;
}

void startGame() {
    for (int i = 1; i < 11; i++){
        for (int j = 1; j < 11; j++){
            BOARD[i][j] = '.';
        }
    }
    PLAYER_1 = malloc(sizeof(player_t));
    PLAYER_1->turnNum = 0;
    PLAYER_1->status = 0;
    PLAYER_1->hasLivingKing = 1;
    PLAYER_1->pawns = NULL;
    PLAYER_1->lastPawn = PLAYER_1->pawns;

    PLAYER_2 = malloc(sizeof(player_t));
    PLAYER_2->turnNum = 0;
    PLAYER_2->status = 0;
    PLAYER_2->hasLivingKing = 1;
    PLAYER_2->pawns = NULL;
    PLAYER_2->lastPawn = PLAYER_2->pawns;
}

void endGame() {
    tearDownPawns(&PLAYER_1->pawns);
    tearDownPawns(&PLAYER_2->pawns);
    PLAYER_1->lastPawn = NULL;
    PLAYER_2->lastPawn = NULL;
    free(PLAYER_1);
    free(PLAYER_2);
}

int gameStatus(){
    if (PLAYER_1->turnNum >= MAX_TURNS_PER_PLAYER
        && PLAYER_2->turnNum >= MAX_TURNS_PER_PLAYER){
        fprintf(stderr, "draw\n");
        return 2; //end of the game
    }
    if (PLAYER_1->hasLivingKing == 0 && PLAYER_2->hasLivingKing == 0){
        fprintf(stderr, "draw\n");
        return -1; //its a draw
    }
    else if (PLAYER_1->hasLivingKing == 0){
        fprintf(stderr, "player 2 won\n");
        return 1; //PLAYER_2 won
    }
    else if (PLAYER_2->hasLivingKing == 0){
        fprintf(stderr, "player 1 won\n");
        return 0; //PLAYER_1 won
    }
    return -2;
}

int init(int n, int k, int p, int x1, int y1, int x2, int y2) {
    BOARD_SIZE = n;
    MAX_TURNS_PER_PLAYER = k;
    if (x1 < 1 || x2 < 1 || y1 < 1 || y2 < 1
        || x1+3 > n || x2+3 > n || y1 > n || y2 > n
        || (abs(x1 - x2) + abs(y1 - y2) < 8)){
        return 0;
    }
    if (p == 1 || p == 2){
        addPawn(&PLAYER_1, 'K', x1, y1);
        addPawn(&PLAYER_1, 'C', x1+1, y1);
        addPawn(&PLAYER_1, 'R', x1+2, y1);
        addPawn(&PLAYER_1, 'R', x1+3, y1);
        addPawn(&PLAYER_2, 'k', x2, y2);
        addPawn(&PLAYER_2, 'c', x2+1, y2);
        addPawn(&PLAYER_2, 'r', x2+2, y2);
        addPawn(&PLAYER_2, 'r', x2+3, y2);
        updateBoard(x1, y1, x2+3, y2+3);
        return 1;
    }
    return 0;
}

/**
    Simulation of a fight between two pawns.
    Pawn who lost is automatically removed from list of pawns.
    @param[in] first First pawn.
    @param[in] second Second pawn.
    @return NULL When both of pawns died.
    @return first When first pawn won.
    @return second When second pawn won.
    */
pawn_t* fight(pawn_t **first, pawn_t **second){
    // x vs x - both die, where x can be any role
    // r vs x - knight wins with everyone (if he's not fighting against another knight)
    // k vs c - king wins against peasant
    char roleFirst = (*first)->role, roleSecond = (*second)->role;
    if ((*first)->role <= 'Z'){
        roleFirst = (char)((int)(*first)->role + 32); //converting to lowercase character
    }
    if ((*second)->role <= 'Z'){
        roleSecond = (char)((int)(*second)->role + 32); //converting to lowercase character
    }
    if (roleFirst == roleSecond) { //both die
        removePawn(first);
        removePawn(second);
        return NULL;
    }
    else if (roleFirst == 'r') {  //knight wins with everyone
        removePawn(second);
        return *first;
    }
    else if (roleSecond == 'r') { //knight wins with everyone
        removePawn(first);
        return *second;
    }
    else if (roleFirst == 'k') {  //second is not a knight, so the king wins with a peasant
        removePawn(second);
        return *first;
    }
    else if (roleSecond == 'k') { //first is not a knight nor a king so he's a peasant
        removePawn(first);
        return *second;
    }
    else{
        return NULL; //undefined behaviour?
    }
}

int move(int playerNum, int x1, int y1, int x2, int y2) {
    if (validateCoordinates(x1, y1, x2, y2) == 0){
        return 0; //incorrect move
    }
    int targetPlayerNum;
    if (playerNum == 0)
        targetPlayerNum = 1;
    else
        targetPlayerNum = 0;
    pawn_t *pawn = findPawn(playerNum, x1, y1);
    pawn_t *target = findPawn(targetPlayerNum, x2, y2); //is enemy standing on destination point
    pawn_t *myPawn = NULL;
    if (target == NULL){
        myPawn = findPawn(playerNum, x2, y2); //is my own pawn standing on destination point
    }
    //preventing from moving other players pawns
    if (pawn == NULL || (playerNum == 0 && pawn->role >= 'a')
            || (playerNum == 1 && pawn->role <= 'Z')){
        return 0;
    }
    if (pawn->lastMove == GLOBAL_TURN){
        return 0; //already moved in this turn
    }
    if (target != NULL) { // pawns fight
        pawn_t *winner = fight(&pawn, &target); //returns winner and removes the loser
        if (winner != NULL) {  //winner == NULL if both died
            if (winner->role == pawn->role) {//if pawn(winner) won then move pawn
                //if pawn(attacker) lost, don't move target(winner)
                movePawn(&winner, x2, y2);
            }
        }
    }
    else if (myPawn != NULL){
        return 0; //can't step on my own pawn
    }
    else{ //just move it because nothing stays in the way
        movePawn(&pawn, x2, y2);
    }
    updateBoard(x1, y1, x2, y2);
    return 1; //everything is alright
}

/**
    Produces an unit with given specification.
    @param[in] role Role of prduced unit.
    @param[in] playerNum Number of the player whose unit is produced.
    @param[in] x1 Column number of producing unit.
    @param[in] y1 Row number of producing unit.
    @param[in] x2 Column number of produced unit.
    @param[in] y2 Row number of produced unit.
    @return 0 If player with number playerNum does not exists.
    @return 1 If pawn was successfully spawned.
    */
int produceUnit(char role, int playerNum, int x1, int y1, int x2, int y2){
    player_t **player;
    int targetPlayerNum;

    if (playerNum == 0){
        player = &PLAYER_1;
        targetPlayerNum = 1;
    }
    else if (playerNum == 1){
        player = &PLAYER_2;
        targetPlayerNum = 0;
    }
    else{
        return 0;
    }

    pawn_t *peasant = findPawn(playerNum, x1, y1);
    pawn_t *target = findPawn(targetPlayerNum, x2, y2);
    pawn_t *myPawn = NULL;
    if (target == NULL){
        myPawn = findPawn(playerNum, x2, y2);
    }
    if (peasant != NULL && target == NULL && myPawn == NULL
        && (peasant->lastMove + 3) == GLOBAL_TURN){
        if (peasant->role == 'C' || peasant->role == 'c'){ //checking if its really peasant
            addPawn(player, role, x2, y2);
            movePawn(&peasant, x1, y1);
            updateBoard(x1, y1, x2, y2);
            return 1;
        }
        return 0;
    }
    return 0;
}

int produceKnight(int playerNum, int x1, int y1, int x2, int y2) {
    if (validateCoordinates(x1, y1, x2, y2) == 0){
        return 0; //incorrect move
    }
    if (playerNum == 0){
        return produceUnit('R', playerNum, x1, y1, x2, y2);
    }
    else if (playerNum == 1){
        return produceUnit('r', playerNum, x1, y1, x2, y2);
    }
    return 0;
}

int producePeasant(int playerNum, int x1, int y1, int x2, int y2) {
    if (validateCoordinates(x1, y1, x2, y2) == 0){
        return 0; //incorrect move
    }
    if (playerNum == 0){
        return produceUnit('C', playerNum, x1, y1, x2, y2);
    }
    else if (playerNum == 1){
        return produceUnit('c', playerNum, x1, y1, x2, y2);
    }
    return 0;
}

void endTurn(int playerNum) {
    if (playerNum == 0){
        PLAYER_1->status = 1;
    }
    if (playerNum == 1){
        PLAYER_2->status = 1;
    }
    if (PLAYER_1->status + PLAYER_2->status == 2){
        GLOBAL_TURN++;
        setStatusNewTurn(GLOBAL_TURN);
    }
}
/**
    @}
    */
