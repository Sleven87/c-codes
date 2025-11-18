#include <stdio.h>

#define WIDTH 40
#define HEIGHT 20

int paddleY = HEIGHT /2;    //middle of board
int ballX = WIDTH /2;
int ballY = HEIGHT /2;      
int ballDirX = 1;           //moving right
int ballDirY = 1;           //moving down



#ifdef _WIN32   //if statement that figures out if windows or linux and then uses the appropriate sleep functionality for whichever is running the app.
#include <windows.h>

void sleep_ms(int ms){
    sleep(ms); //Sleep takes milliseconds
}
#else
#include <unistd.h>
void sleep_ms(int ms){
    usleep(ms * 1000); //usleep takes microseconds
}
#endif


void clearScreen(){ //create a screenclear function that works on both windows and linux and the game will figure out what system it is running in and then use the appropriate screen clear function
    #ifdef _WIN32
        system("cls");
    #else
        printf("\033[H\033[J");
    #endif
}


#ifdef _WIN32
#include <conio.h>

int keyPressed(){
    if (_kbhit())
        return _getch();
    return 0;
}

#else
#include <termios.h>
#include <sys/select.h>

int keyPressed(){
    struct timeval tv = {0L, 0L};
    fd_set fds;

    FD_ZERO(&fds);
    FD_SET(0, &fds);

    int lastKey = 0;

    while(select(1, &fds, NULL, NULL, &tv) > 0){
        int c = getchar();
        if (c != EOF)
            lastKey = c;

    }
        return lastKey;     //0 if no key was pressed
    }
#endif




void drawBoard(){

    // TOP WALL
    for(int i = 0; i < WIDTH +2; i++)
        printf("#");
    printf("\n");
        
    
    //MIDDLE AREA
    for(int y = 0; y < HEIGHT; y++){
        
        printf("#"); //LEFT WALL
        
        for (int x = 0; x < WIDTH; x++){
            
            //paddle (3 blocks tall)
            if (x == 1 && (y == paddleY || y == paddleY - 1 || y == paddleY +1)){
            printf("|");

            //ball
            }else if (x == ballX && y == ballY){
                printf("O");
                
            }
            
            //empty
            else
            printf(" "); //SPACER
            
        }
                
        printf("#\n"); //RIGHT WALL
    }
        
    //BOTTOM WALL
    for(int i = 0; i < WIDTH +2; i++)
        printf("#");
    printf("\n");


    
}

#ifndef _WIN32
#include <termios.h>
struct termios old_tio, new_tio;

void initTermios(){                     //unlocks the terminal so that it is able to look for keypresses without hitting enter afterwards.
    tcgetattr(0, &old_tio);             // get current terminal settings
    new_tio = old_tio;
    new_tio.c_lflag &= ~ICANON;         //disable canonical mode (no Enter needed)
    new_tio.c_lflag &= ~ECHO;           //disable echo
    tcsetattr(0, TCSANOW, &new_tio);    //apply new settings
}

void resetTermios(){
    tcsetattr(0, TCSANOW, &old_tio);    //restore old settings
}
#endif


int main(){


    
    #ifndef _WIN32
        initTermios();
    #endif


    clearScreen();
    //printf("\033[H\033[J");
    drawBoard();

while(1){

    int key = keyPressed();         //will be 0 if no key pressed
    
    if (key == 'w' && paddleY > 1)
        paddleY--;

    if (key == 's' && paddleY < HEIGHT -2)
        paddleY++;

    int ballNextX = ballX + ballDirX;          //the next x location where ball will be
    int ballNextY = ballY + ballDirY;          //the next y location where ball will be

    //paddle collision
    if (ballNextX == 1){                //check if ball is moving to a location where the paddle exists
        if (ballNextY == paddleY ||
            ballNextY == paddleY -1 ||
            ballNextY == paddleY +1)
        {
            ballDirX *= -1;             //bounce ball
        }
    }

    //bounce on top/bottom
    if (ballNextY < 0 || ballNextY > HEIGHT -1)
        ballDirY *= -1;

    //bounce ball on left/right ONLY USED FOR TESTING SO BALL DOESNT DISSAPEAR
    if (ballNextX <= 0 || ballNextX >= WIDTH -1)
        ballDirX *= -1;

    
    //MOVE THE BALL 
    ballX += ballDirX;
    ballY += ballDirY;

    //clear screen
    clearScreen();
    //printf("\033[H\033[J");

    //draw the new frame
    drawBoard();
    

    //delay
    sleep_ms(100);
}

    #ifndef _WIN32
        resetTermios();
    #endif

    return 0;

}