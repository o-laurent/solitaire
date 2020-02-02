#include <stdio.h>
#include <stdlib.h>

#include "structures.h"
#include "main.h"



void initBoard(state board[7][7]) {
    //Initialise the board to an English solitaire
    for (int i=0; i<7; i++) {
        for (int j=0; j<7; j++) {
            if ( (i%6==0 && j%6==0) || (i%6==0 && j==1) || (i==1 && j%6==0) || (i%6==0 && j==5) || (i==5 && j%6==0) ) {
                board[i][j] = out;
            }
            else if ( i==j && i==3 ) {
                board[i][j] = empty;
            }
            else {
                board[i][j] = ball;
            }
        }
    }
}

int ballNb(state board[7][7]) {
    //Counts the number of balls on the board
    int count =0;
    for (int i=0; i<7; i++) {
        for (int j=0 ; j<7 ; j++) {
            if (board[i][j]==ball) {
                count++;
            }
        }
    }
    return count;
}

int correctMove(state board[7][7], movement* move) {
    int ok;
    //Check if the position is ball and if the next positions are empty
    int x = move->posix;
    int y = move->posiy;
    direction dir = move->dir;
    if (board[x][y]==ball){
        if ((dir==north) && (board[x-1][y]==ball) && (board[x-2][y]==empty)){
            ok = 1;
        }
        else if ((dir==south) && (board[x+1][y]==ball) && (board[x+2][y]==empty)){
            ok = 1;
        }
        else if ((dir==east) && (board[x][y+1]==ball) && (board[x][y+2]==empty)){
            ok = 1;
        }
        else if ((dir==west) && (board[x][y-1]==ball) && (board[x][y-2]==empty)){
            ok = 1;
        }
        else {
            ok = 0;
        }
    }
    else {
        ok = 0;
        printf("NOT A BALL");
    }
    return ok;
}

int possibleMove(state board[7][7]) {
    //Returns 1 if one move can still be done
    int doable;
    if (ballNb(board) >= 2){
        //If there is only one ball, one can not play anymore
        doable = 1;
        //NOT FINISHED
    }
    else {
        doable = 0;
    }
    return doable;
}   

void doMove(state board[7][7], movement* move) {
    //Makes the move
    //You must check that the move is correct before calling this function !
    int x = move->posix;
    int y = move->posiy;
    if (move->dir==north) {
        board[x][y] = empty;
        board[x-1][y] = empty;
        board[x-2][y] = ball;
    }
    else if (move->dir==south) {
        board[x][y] = empty;
        board[x+1][y] = empty;
        board[x+2][y] = ball;
    }
    else if (move->dir==east) {
        board[x][y] = empty;
        board[x][y+1] = empty;
        board[x][y+2] = ball;
    }
    else if (move->dir==west) {
        board[x][y] = empty;
        board[x][y-1] = empty;
        board[x][y-2] = ball;
    }
    else {
        printf("UNEXPECTED ERROR");
    }
}

void userMove(state board[7][7]) {
    //Allows the user to chose the movement he desires
    char dir;
    char line[1024];
    movement move;
    int status = 0; //Will be true if a correct movement is recorded
    while (!status) {
        printf("Entrez la coordonnée verticale de la balle à déplacer\n");
        fgets(line,1024,stdin);
        sscanf(line,"%hhd",&(move.posix));

        printf("Entrez la coordonnée horizontale de la balle à déplacer\n");
        fgets(line,1024,stdin);
        sscanf(line,"%hhd",&(move.posiy));
        
        int ok = 0;
        while(!ok) {
            printf("Entrez la direction du mouvement (n,s,e,o) : \n");
            fgets(line,1024,stdin);
            sscanf(line,"%c",&dir);
            if (dir=='n') {
                ok++;
                move.dir = north;
            }
            else if (dir=='s') {
                ok++;
                move.dir = south;
            }
            else if (dir=='e') {
                ok++;
                move.dir = east;
            }
            else if (dir=='o') {
                ok++;
                move.dir = west;
            }
            else {
                ok =0;
                printf("Erreur lors de l'entrée \n");
            }
        }
        status = correctMove(board, &move);
        printf("%d \n",status);
        if (status==0){
            printf("Ce mouvement n'est pas possible. Veuillez en proposer un autre :\n");
        } 
    }
    doMove(board, &move);
    
}

void printBoard(state board[7][7]) {
    printf("\n");
    for (int i=0 ; i<7 ; i++) {
        if (i==0) {
            printf("  ");
            printf("\033[0;31m"); //Red
            for (int j=0 ; j<7 ; j++) {
                printf("%d ", j);
            }
            printf("\033[0m");
            printf("\n");
        }
        
        for (int j=0 ; j<7 ; j++) {
            if (j==0) {
                printf("\033[0;34m"); //Blue
                printf("%d ", i);
                printf("\033[0m");
            }
            if (board[i][j]==ball) {
                printf("O ");
            }
            else if (board[i][j]==empty) {
                printf("\033[37;2m"); //Grey 
                printf("X ");
                printf("\033[0m");
            }
            else {
                printf("  ");
            }
        }
        printf("\n");
    }
    printf("\n");
}

void userGame() {
    //Define 7x7 board and a counter
    state board[7][7];
    int turn = 1;

    //Initialise the board
    initBoard(board);

    while (possibleMove(board)){
        printf("----------   Début du tour %d ----------\n", turn);
        //Print the board
        printBoard(board);
        userMove(board);
        turn++;
    }
}

int main(){ 
    unsigned char status;
    char line[1024];
    //intro
    printf("Bienvenue dans le Solitaire v0.1\n");
    printf("\n");

    //mode choice
    printf("Appuyez sur '1' pour jouer ou '2' pour une résolution automatique :\n");
    fgets(line,1024,stdin);
    sscanf(line,"%hhd",&status);

    //checking for input errors
    while (status!=1 && status!=2) {
        printf("Erreur lors de l'entrée. Veuillez réessayer;\n");
        printf("Appuyez sur '1' pour jouer ou '2' pour une résolution automatique :\n");
        fgets(line,1024,stdin);
        sscanf(line,"%hhd",&status);
    }

    //actual game
    if (status==1) {
        userGame();
    }

    printf("\n");
    printf("Développé par Anthony Aoun, Maria El Haddad, Olivier Laurent et Johhny Yammine dans le cadre du projet de IN103 : Algorithmique en C \n");

    printf("\n");
    printf("Merci d'avoir joué. Au revoir ^~^\n");
    return 0;
}