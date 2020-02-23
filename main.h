#ifndef _MAIN_H_
#define _MAIN_H_
#include "structures.h"

//main.c
int makePossibleMoves(state **board, movementList* moveList);
int moveNb(state **board);
void initBoard(state **board);
int ballNb(state **board);
int correctMove(state **board, movement* move);
int possibleMove(state **board);
void doMove(state **board, movement* move);
void userMove(state **board, int* pquit);
void printBoard(state **board);
int userGame(int* pquit, trajectory** pTrajectory, state** board, int* turn);

//data.c
void saveGame (state** board, int turn, double time);
void rmSavedGame();
void loadGame (state** board, long long int returned[2]);
int isThereASavedGame();
int readNumberOfGames();
double totalPlayedTime();
void implementStats(double time);

//uiBoard.c
void printBoardV(state **board, char lineNb, char colNb);
void printTrajectory(trajectory* trajOrigin);
void rmTrajectory();
void saveTrajectory(trajectory* trajOrigin);
state** readBoard (char* fileName, char* lineNb, char* colNb);

//rules.c
void printRules();

//autosolve.c
trajectoryNode* autosolve(trajectoryNode* pTrajectory, int* boardNb);


typedef struct node node;
movementList* consML(movement* move, movementList* moveList);

void copyBoard(state** boardI, state** boardO) {
    for(int i=0;i<7;i++){
        for(int j=0;j<7;j++){
            boardO[i][j] = boardI[i][j];
        }
    }
}
#endif