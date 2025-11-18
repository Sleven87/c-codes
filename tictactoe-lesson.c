#include <stdio.h>
#include <stdlib.h> //required to use system clear
#include <time.h>
#include <unistd.h>


void printBoard(char board[]);
int checkWinner(const char board[], char player);
int isBoardFull(const char board[]);
int getAIMove_random(const char board[]);
int getAIMove_blockwin(const char board[], char ai, char human); // heuristic AI


void printSymbol(char c) {
    if (c == 'X') {
        printf("\033[32mX\033[0m");  // Green
    } else if (c == 'O') {
        printf("\033[34mO\033[0m");  // Blue
    } else {
        printf("%c", c);             // Numbers stay default color
    }
}

void printBoard(char board[]){
    //prints the board grid using the characters in the array
    printf(" "); printSymbol(board[0]); printf(" | "); printSymbol(board[1]); printf(" | "); printSymbol(board[2]);
    printf("\n---|---|---\n");
    printf(" "); printSymbol(board[3]); printf(" | "); printSymbol(board[4]); printf(" | "); printSymbol(board[5]);
    printf("\n---|---|---\n");
    printf(" "); printSymbol(board[6]); printf(" | "); printSymbol(board[7]); printf(" | "); printSymbol(board[8]);printf("\n");
}

int getAIMove_random(const char board[]){
    int empties[9];
    int count = 0;
    for (int i = 0; i < 9; i++){
        if (board[i] != 'X' && board[i] != 'O'){
            empties[count++] = i;
        }
    }
    if (count == 0) return -1;
    int idx = rand() % count;
    return empties[idx]; //returns index 0...8
}

//Try to find a winning move for 'player', Returns index (0..8) or -1.
int findWinningMove(const char board[], char player){
    char temp[9];
    for (int i = 0; i < 9; i++) temp[i] = board[i];

    for (int i = 0; i < 9; i++) {
        if (temp[i] != 'X' && temp[i] != 'O') {
            temp[i] = player;
            if (checkWinner(temp, player)) return i;
            temp[i] = '1' +i; //restore placeholder (or board[i] is fine)
        }
    }
    return -1;
}

int getAIMove_blockwin(const char board[], char ai, char human) {
    int move;
    // 1) Win if possible
    move = findWinningMove(board, ai);
    if (move != -1) return move;
    // 2) Block oponent
    move = findWinningMove(board, human);
    if (move != -1) return move;
    // 3) otherwise random
    return getAIMove_random(board);
}

int isBoardFull(const char board[]){
    for (int i = 0; i < 9; i++) {
        if (board[i] != 'X' && board[i] != 'O')
            return 0;
    }
    return 1;
}

int checkWinner(const char board[], char player) {
    static const int wins[8][3] = {
        {0,1,2}, {3,4,5}, {6,7,8},
        {0,3,6}, {1,4,7}, {2,5,8},
        {0,4,8}, {2,4,6}
    };
    
    for (int i = 0; i < 8; i++) {
        if (board[wins[i][0]] == player &&
            board[wins[i][1]] == player &&
            board[wins[i][2]] == player) {
            return 1; // player has won
        }
    }
    return 0; // no win
}


int main(){
    
    int playing = 1; //1 = playing and 0 = not playing
    
    srand((unsigned)time(NULL));


    while(playing){
        int gameOver = 0; //0 = game running and 1 = game ended
        char board[9]; //size of the tic tac toe board
        char currentPlayer = 'X'; //setting the current player to start as X

        //reset board numbers
        for (int i = 0; i < 9; i++)
            board[i] = '1' +i; // '1', '2', '3' ... '9'
        
        //---------------Game Loop-------------
        while (!gameOver) {
            // game logic here
            // human turn, AI turn, checking winner, etc.
            //clear the screen before printing the current gamestate
            printf("\033[2J\033[H"); //use system("cls"); if you're on Windows.
            fflush(stdout);
            printBoard(board); //Prints the current state of the board

            if(currentPlayer == 'X'){
                //Human turn
                int valid = 0;
                int choice;

                do {
                    printf("Player %c, Enter a number (1-9) to place your %c: ", currentPlayer, currentPlayer);
                    if (scanf("%d", &choice) !=1) {
                        //bad input - clear stdin
                        int c;
                        while ((c = getchar()) != '\n' && c != EOF) {}
                        printf("Invalid input. Please enter a number 1-9.\n");
                        continue;
                    }

                    if (choice <1 || choice >9) {
                        printf("Choice out of range. Enter 1-9.\n");
                        continue;
                    }

                    if (board[choice-1] == 'X' || board[choice-1] == 'O') {
                        printf("Cell already taken! Try again.\n");
                        continue;
                    }
                    
                    board[choice-1] = currentPlayer; //place the players mark
                    valid = 1;
                } while (!valid);
            } else {
                //AI's turn (0)
                printf("AI (%c) is thinking...\n", currentPlayer);
                //wait 1 second
                sleep(1);
                //Choose AI strategy here; try block/win heuristic:
                int aiMove = getAIMove_blockwin(board, 'O', 'X'); //smart ai
                //int aiMove = getAIMove_random(board); //dumb ai
                if (aiMove == -1) {
                    //no move available (shouldent happen because we check fullness earlier)
                    gameOver = 1;
                    break;
                }
                board[aiMove] = currentPlayer;
            }

            //after the move, check win/draw
            if (checkWinner(board, currentPlayer)){
                printf("\033[2J\033[H");
                fflush(stdout);
                printBoard(board);
                if (currentPlayer == 'X') printf("You (X) Win! Game Over!\n");
                else printf("AI (O) Wins! Game Over!\n");
                gameOver = 1;
                break;
            }

            if (isBoardFull(board)) {
                
                //game over screen
                printf("\033[2J\033[H");
                fflush(stdout);
                printBoard(board);
                printf("Draw! No more moves.\n");
                gameOver = 1;
                break;

            }

            //switch player
            currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
        
        }


        // -------------rematch code-------------
        int choice; //variable to take in the players inputted choice number 1-9
        int valid = 0;

        do {
            printf("Game Over do you want to play again? (1 = Yes, 2 = No)\n");
            if (scanf("%d", &choice) !=1) {
            //bad input - clear stdin
                int c;
                while ((c = getchar()) != '\n' && c != EOF) {}
                    printf("Invalid input. Please enter 1 for new game or 2 to quit.\n");
                    continue;
                }

                if (choice == 1) {
                    valid = 1; //loop continues naturally
                }else if (choice == 2 ) {
                    playing = 0; //end outer loop                    
                    valid = 1;
                } else {
                    printf("Enter 1 or 2.\n");
                }
                
            } while (!valid);
        }

        return 0;
}