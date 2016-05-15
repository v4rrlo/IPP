/** @file
    Interface of game engine.
    
    @author Miłosz Rzeźnikowski <mr371647@students.mimuw.edu.pl>
    @copyright Uniwersytet Warszawski
    @date 2016-05-15
    */

#ifndef ENGINE_H
#define ENGINE_H

/**
    Initializes a game. Needed before first INIT.
    */
void startGame();

/**
    Frees memory. Needed after finishing game.
    */
void endGame();

/**
    Indicates a status of the game and prints the result.
    -2 - game still going on;
    -1 - draw;
    0 - player1 won;
    1 - player2 won;
    2 - timeout
    */
int gameStatus();

/**
    Initializes a game with size of a board, number of rounds and positions of kings.
    */
int init(int n, int k, int p, int x1, int y1, int x2, int y2);

/**
    Makes a move.
    @param[in] playerNum Number of the player who makes a move.
    @param[in] x1 Column number before a move.
    @param[in] y1 Row number before a move.
    @param[in] x2 Column number after a move.
    @param[in] y2 Row number after a move.
    @return 0.
    */
int move(int playerNum, int x1, int y1, int x2, int y2);

/**
    Produces a Knight unit.
    @param[in] playerNum Number of the player who makes an action.
    @param[in] x1 Column number of Peasant unit.
    @param[in] y1 Row number of Peasant unit.
    @param[in] x2 Column number of destination of produced Knight.
    @param[in] y2 Row number of destination of produced Knight.
    @return 0 if coordinates or playerNum are invalid
    @return 1 if everything is ok
    */
int produceKnight(int playerNum, int x1, int y1, int x2, int y2);

/**
    Produces a Peasant unit.
    @param[in] playerNum Number of the player who makes an action.
    @param[in] x1 Column number of Peasant unit.
    @param[in] y1 Row number of Peasant unit.
    @param[in] x2 Column number of destination of produced Peasant.
    @param[in] y2 Row number of destination of produced Peasant.
    @return 0 if coordinates or playerNum are invalid
    @return 1 if everything is ok
    */
int producePeasant(int playerNum, int x1, int y1, int x2, int y2);

/**
    Ends turn of playerNum and indicates that he has ended the turn.
    When both players ended their turns, global count of turns is incremented \
    and statuses of players changed back to 'turn not ended'.
    @param[in] playerNum Number of the player who ends his turn.
    */
void endTurn(int playerNum);

/**
    Prints (into stdout) top-left corner of the board of size m x m where m = min(n,10).
    */
void printTopLeft();

#endif /* ENGINE_H */
