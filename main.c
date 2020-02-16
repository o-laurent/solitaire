#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "main.h"

state* MatrixToVector(state matr[7][7]) {
	state* vect = malloc(sizeof(state)*49);
	int i;
	int j;

	for(i=0;i<7;i++) {
		for(j=0;j<7;j++) {
			vect[i*7 + j]=matr[i][j];
        }
    }
    return vect;
}

state** VectorToMatrix(state vect[49]) {
    state **matr = malloc(sizeof(*matr) * 7);
    for (int i=0;i<7;i++) {
        matr[i] = malloc(sizeof(**matr)*7);
    }
    int i;
    int j;

    for(i=0;i<7;i++) {
        for(j=0;j<7;j++) {
            matr[i][j]=vect[i*7+j];
        }
    }
    return matr;
}


int makePossibleMoves(state **board, movementList* moveList) {
    //Generates the linked list which contains all the moves
    int i = 0;
    int j = 0;
    int k = 0;
    int nb = 0;
    movement move;
    for (i=0 ; i<7 ; i++) {
        for (j=0 ; j<7 ; j++) {
            if (board[i][j]==ball) {
                move.posix = i;
                move.posiy = j;
                for (k=0 ; k<4 ; k++) {
                    move.dir = k;
                    if (correctMove(board, &move)) {
                        consML(&move, moveList);
                        nb++;
                    }
                }
            }
        }
    }
    return nb;
}

int moveNb(state **board) {
    //Returns the number of possible moves
    int nb;
    movementList moveList;
    nb = makePossibleMoves(board, &moveList);
    return nb;
}

int moveFixed(state **board, movementList** moveList, unsigned char x, unsigned char y) {
    //Returns the moves
    int nb = 0;
    int k = 0;
    movement move;
    if (board[x][y]==ball) {
        move.posix = x;
        move.posiy = y;
        for (k=0 ; k<4 ; k++) {
            move.dir = k;
            if (correctMove(board, &move)) {
                *moveList = consML(&move, *moveList);
                nb++;
            }
        }
    }
    move = (*moveList)->move;
    return nb;
}

void initBoard(state **board) {
    //Initialise the board to a English solitaire
    for (int i=0; i<7; i++) {
        for (int j=0; j<7; j++) {
            if ((i%6==0 && j%6==0) || (i%6==0 && j==1) || (i==1 && j%6==0) || (i%6==0 && j==5) || (i==5 && j%6==0)) {
                board[i][j] = out;
            }
            else if (i==j && i==3) {
                board[i][j] = empty; 
            }
            else {
                board[i][j] = ball;
            }
        }
    }
}

int ballNb(state **board) {
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

int correctMove(state **board, movement* move) {
    int ok;
    //Check if the position is ball and if the next positions are empty
    int x = move->posix;
    int y = move->posiy;
    direction dir = move->dir;
    if (board[x][y]==ball){
        if ((dir==north) && x!=0 && x!=1 && (board[x-1][y]==ball) && (board[x-2][y]==empty)) {
            ok = 1;
        }
        else if ((dir==south) && x!=5 && x!=6 && (board[x+1][y]==ball) && (board[x+2][y]==empty)) {
            ok = 1;
        }
        else if ((dir==east) && y!=5 && y!=6 && (board[x][y+1]==ball) && (board[x][y+2]==empty)) {
            ok = 1;
        }
        else if ((dir==west) && y!=0 && y!=1 && (board[x][y-1]==ball) && (board[x][y-2]==empty)) {
            ok = 1;
        }
        else {
            ok = 0;
        }
    }
    else {
        ok = 0;
        printf("UNEXPECTED ERROR 1");
    }
    return ok;    
}

int possibleMove(state **board) {
    //Returns 1 if one move can still be done
    int doable;
    if ((ballNb(board)>=2) && (moveNb(board)>=1)) {
        //ballNb for optimality (allows not to go through moveNb)
        doable = 1;
    }
    else {
        //If there is only one ball, one can not play anymore
        doable = 0;
    }
    return doable;
}   

void doMove(state **board, movement* move) {
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

void userMove(state **board, int* pquit) {
    //Allows the user to chose the movement he desires
    char dir;
    char line[1024];
    movement move;
    int goodMove = 0; //True if a correct movement is recorded
    movementList* moveList = malloc(sizeof(movementList));
    while (!goodMove) {
        //Print the board
        printBoard(board);    
        printf("Entrez la coordonnée ");
        printf("\033[0;34m");
        printf("verticale ");
        printf("\033[0m");
        printf("de la balle à déplacer\n");
        fgets(line, 1024, stdin);
        sscanf(line, "%hhd", &(move.posix));
        
        //if we don't want to quit
        if (move.posix != 9) {

            printf("Entrez la coordonnée ");
            printf("\033[0;31m");
            printf("horizontale ");
            printf("\033[0m");
            printf("de la balle à déplacer\n");
            fgets(line, 1024, stdin);
            sscanf(line, "%hhd", &(move.posiy));
            printf("%hhd \n",move.posix);
            //if we don't want to quit
            if (move.posiy != 9) {

                printf("\n");

                int ok = 0;
                if (moveFixed(board, &moveList, move.posix, move.posiy)==1) {
                    ok = 1;
                    move = moveList->move;
                    goodMove = correctMove(board, &move);
                }
                
                else if (board[move.posix][move.posiy]==ball) {
                    while(!ok && !goodMove) {
                        printf("Entrez la direction du mouvement (n, s, e, o) : \n");
                        fgets(line, 1024, stdin);
                        sscanf(line, "%c", &dir);

                        //if we don't want to quit
                        if (dir != 9) {

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
                        
                            goodMove = correctMove(board, &move);
                            printf("%d \n", goodMove);
                            if (goodMove==0){
                                printf("\033[1;31m"); //Red
                                printf("Erreur utilisateur\n");
                                printf("\033[0m");
                                sleep(0.5);
                                printf("Ce mouvement n'est pas possible. Veuillez en proposer un autre :\n");
                                sleep(1.5);
                            } 
                        }  
                        else if (dir == 9) {
                            *pquit = 1;
                            goodMove = 1;
                        }
                    }
                  
                }
                else {
                    printf("\033[1;31m"); //Red
                    printf("Erreur utilisateur\n");
                    printf("\033[0m");
                    sleep(0.5);
                    printf("Le mouvement proposé ne correspond pas à une bille.\nVeuillez en proposer un autre :\n");
                    sleep(1.5);
                }
            }
            else if (move.posiy == 9) {
                *pquit = 1;
                goodMove = 1;
            }
        }
        else if (move.posix == 9) {
            *pquit = 1;
            goodMove = 1;
        }
    }
    if (*pquit==0) {
        doMove(board, &move);
    }
}

void printBoard(state **board) {
    printf("\n");
    for (int i=0 ; i<8 ; i++) {
        if (i==0 || i==7) {
            printf("  ");
            printf("\033[0;31m"); //Red
            for (int j=0 ; j<7 ; j++) {
                printf("%d ", j);
            }
            printf("\033[0m");
            printf("\n");
        }
        for (int j=0 ; j<8 ; j++) {
            if ((j==0 || j==7) && i!=7) {
                printf("\033[0;34m"); //Blue
                printf("%d ", i);
                printf("\033[0m");
            }
            if (i<7 && j<7 && board[i][j]==ball) {
                printf("\033[1m");
                printf("o ");
                printf("\033[0m");
            }
            else if (i<7 && j<7 && board[i][j]==empty) {
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

int userGame(int* pquit) {
    //Define 7x7 board and a counter
    trajectory uTrajectory;
    state **board = malloc(sizeof(*board) * 7);
    for (int i=0;i<7;i++) {
        board[i] = malloc(sizeof(**board)*7);
    }
    int turn = 1;

    //Initialise the board
    initBoard(board);
    uTrajectory ;
    while (possibleMove(board) && (!(*pquit))){
        printf("----------   Début du tour %d ----------\n", turn);
        //
        userMove(board, pquit);
        //
        turn++;
    }
    return ballNb(board);
}

int main(){ 
    unsigned char status;
    time_t secondsStart; 
    time_t secondsEnd; 
    char line[1024];
    int ballNumber = 36;
    int quit = 0;
    int* pquit = &quit;

    //intro
    printf("Bienvenue dans le Solitaire v1.0\n");
    printf("\n");

    //Chose the mode
    printf("Appuyez sur '0' pour les règles du jeu, '1' pour jouer ou '2' pour une résolution automatique.\nAppuyez à tout moment sur '9' pour quitter la partie :\n");
    fgets(line, 1024, stdin);
    sscanf(line, "%hhd", &status);

    //checking for input errors
    while (status!=1 && status!=2 && status!=9) {
        if (status==0){
            printf("Le solitaire est, comme son nom l’indique, un jeu où l’on n’a aucun adversaire.\n"
            "Le but est de déplacer des billes sur un plateau, de les ôter petit à petit selon une règle précise "
            "jusqu’à ce qu’il ne reste qu’une seule bille au centre.\nLe joueur va prendre une bille et sauter par "
            "dessus la seconde se trouvant à coté pour aller rejoindre le trou vide.\nLa bille qui aura été sautée est alors sortie du jeu.\n"
            "Il faut savoir qu’à chaque coup, le joueur ne peut prendre qu’une seule bille à la fois et que cette dernière est prise par une autre "
            "bille venant verticalement ou horizontalement à elle.\n\n");
	        printf("Appuyez sur '0' pour les règles du jeu, '1' pour jouer ou '2' pour une résolution automatique.\nAppuyez à tout moment sur '9' pour quitter la partie :\n");
	        fgets(line, 1024, stdin);
            sscanf(line, "%hhd", &status);//Tutorial 
	    }
        else {
            printf("Erreur lors de l'entrée. Veuillez réessayer;\n");
            printf("Appuyez sur '0' pour les règles du jeu, '1' pour jouer ou '2' pour une résolution automatique.\nAppuyez à tout moment sur '9' pour quitter la partie :\n");
            fgets(line, 1024, stdin);
            sscanf(line, "%hhd", &status);
        }}

    //game
    if (status==1) {
        char resume;

        //ask if the user wants to load game
        printf("\n");
        printf("Voulez-vous continuer la partie précédente ? (o/n)\n");
        fgets(line, 1024, stdin);
        sscanf(line, "%c", &resume);
        while (resume!='o' && resume!='n') {
            printf("\n");
            printf("Erreur lors de l'entrée. Veuillez réessayer;\n");
            printf("Voulez-vous continuer la partie précédente ? (o/n)\n");
            fgets(line, 1024, stdin);
            sscanf(line, "%c", &resume);
        }
        if (resume=='o') {
            //ouvrir le fichier
            //verifier qu'il n'est pas vide
            //recuperer matrice, nb de tours, temps
            //affecter a toutes les variables les valeurs retirees
            return -1;
        }

        time(&secondsStart); 
        //
        ballNumber = userGame(pquit);
        //
        time(&secondsEnd); 
        if (*pquit!=1) {
            printf("Après %lf minutes, la partie s'est terminée avec %d billes sur le plateau. \n", ((double)secondsEnd-(double)secondsStart)/60, ballNumber);
            printf("Bravo !\n");
        }
        else {
            status = 9;
        }
    }

    if (status==2) {
        -1; //autoresolve
        //show solution or not
    }

    if (status==9 && ballNumber<36) {
        char x;
        //ask the user if he wants to save game
        printf("\n");
        printf("Voulez-vous sauvegarder la partie ? (o/n)\n");
        scanf("%c",&x);
        while (x!='o' && x!='n') {
            printf("\n");
            printf("Erreur lors de l'entrée. Veuillez réessayer;\n");
            printf("Voulez-vous sauvegarder la partie ? (o/n)\n");
            scanf("%c",&x);
        }
        if (x=='o') {
            //ouvrir un fichier
            //y glisser la matrice, le nb de tours et le temps 
            //confirmer que l'enregistrement a reussi
            return -1;
        }
    }
    printf("\n");
    printf("Développé par Anthony Aoun, Maria El Haddad, Olivier Laurent et Johhny Yammine dans le cadre du projet de IN103 : Algorithmique en C. \n\n");

    printf("\n");
    printf("Merci d'avoir joué. Au revoir ^~^\n");

    //free(board);

    return 0;
}