#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <fcntl.h>


#define WIDTH 80
#define HEIGHT 20

int snakeX[100], snakeY[100];
int snakeLength = 3;
//int FoodX = rand() % WIDTH, FoodY = rand() % HEIGHT;

enum Direction {STOP=0, UP, DOWN, LEFT, RIGHT};
enum Direction dir = STOP;

char wall = '#';
char snakeHead = '@';
char snakeBody = 'o';
char food = '%';
int foodX = 10;
int foodY = 10;

int kbhit(void){
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    
    if(ch != EOF){
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
    
}

void drawBoard(){
    //clears screen
    printf("\033[H\033[J");

    //draw ceiling of board
    for (int i = 0; i < WIDTH +2; i++) //+2 for side walls
        printf("%c", wall);
    printf("\n");

    //print left wall
    for (int y = 0; y < HEIGHT; y++) {
        printf("%c", wall); //left wall
        for (int x = 0; x < WIDTH; x++) {
            char c = ' '; // default empty space
            
            
            if (x == snakeX[0] && y == snakeY[0]) {
                c = snakeHead;
            } else {
                //check body
                for (int i = 1; i < snakeLength; i++) {
                    if (x == snakeX[i] && y == snakeY[i]) {
                        c = snakeBody;
                        break;
                    }
                }
            }

            //check food
            if (x == foodX && y == foodY) {
                c = food;
            }

            printf("%c", c);
        }
        
        printf("%c\n", wall); //right wall
        
        if (kbhit()) {
            char key = getchar();
            if (key == 'w') dir = UP;
            else if (key == 's') dir = DOWN;
            else if (key == 'a') dir = LEFT;
            else if (key == 'd') dir = RIGHT;
        }
    }


    //draw floor of board
    for (int i = 0; i < WIDTH +2; i++) //+2 for side walls
        printf("%c", wall);
    printf("\n");
    
}

void moveSnake(){
    //Move the body segments
    for (int i = snakeLength -1; i > 0; i--) {
        snakeX[i] = snakeX[i-1];
        snakeY[i] = snakeY[i-1];
    }
    
    //move the head
    if (dir == UP) snakeY[0]--;
    else if (dir == DOWN) snakeY[0]++;
    else if (dir == LEFT) snakeX[0]--;
    else if (dir == RIGHT) snakeX[0]++;
    
}

int main(){
    int gameOver = 0; //0 = running, 1 = end
    drawBoard();
    
    snakeX[0] = WIDTH / 2;
    snakeY[0] = HEIGHT / 2;
    
    snakeX[1] = snakeX[0] -1;
    snakeY[1] = snakeY[0];
    
    snakeX[2] = snakeX[0] -2;
    snakeY[2] = snakeY[0];

    while(!gameOver){
        if (kbhit()) readInput();
        moveSnake();
        checkCollision();
        drawBoard();
        usleep(100000); // speed
    }
}
