#include <stdio.h>

int main() {
    int myNum = 7, myOtherNum = 54, sum = myNum / myOtherNum + 2;
    float sHit = 12.66;
    char wHore = 'y';

    //assign the value of myOtherNum to my Num
    myNum = myOtherNum;
    sHit = myNum;

    printf("%d\n%f\n%c\n", myNum, sHit, wHore);
    printf("%d", sum);
    return 0;
}