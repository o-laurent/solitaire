#ifndef AUTOSOLVE_H_
#define AUTOSOLVE_H_
#include <stdio.h>
#include <stdlib.h>
#include "structures.h"
#endif

//autosolve.c
trajectoryNode* autosolve(trajectoryNode* pTrajectory, int* boardNb, int* stop, int* nodeAlloc, int* nodeFree, int* boardAlloc, int* boardFree);
trajectoryNode* rmtTN(trajectoryNode* pTrajectory, int* nodeFree, int* boardFree);
trajectoryNode* rmtTN_Node(trajectoryNode* pTrajectory, int* nodeFree, int* boardFree);
//main.c
int correctMove(state **board, movement* move);
void doMove(state **board, movement* move);
int ballNb(state **board, char lineNb, char colNb);
void copyBoard(state** boardI, state** boardO, char lineNb, char colNb);
int moveNb(state **board, char lineNb, char colNb);

//uiBoard.c
void printBoardV(state **board, char lineNb, char colNb);

//current file
int cost_f(state** board);