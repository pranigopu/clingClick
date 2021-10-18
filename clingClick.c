#include "support/__CUNCTUS__.h"
int message(int);
void regen(int);
int main()
{
    char c; 
    int i;
    if(load() == 0)
    {
        printf("Looks like you cannot play now :(\n");
        return 0;
    }
    system("/bin/stty raw");
    while(1)
    {
        console(itemheld);
        if(player() == -1)
            break;
        health[C] -= dmg;
        injury[C] += dmg;
        regen(P);
        if(health[C] <= 0)
            return message(1);
        console(itemheld);
        comp();
        health[P] -=dmg;
        injury[P] += dmg;
        regen(C);
        if(health[P] <= 0)
            return message(2);
    } 
    gotoxy(0, y_max + 5);
    system("/bin/stty cooked");
    return 0;
}

int message(int option)
{
    console(itemheld);
    gotoxy(x_max / 2 - 3, y_max / 2 - 2);
    printf("*************");
    gotoxy(x_max / 2 - 3, y_max / 2 - 1);
    printf("*           *");
    gotoxy(x_max / 2 - 3, y_max / 2);
    if(option == 1)
        printf("*  VICTORY  *");
    else if(option == 2)
        printf("*  DEFEAT!  *");
    gotoxy(x_max / 2 - 3, y_max / 2 + 1);
    printf("*           *");
    gotoxy(x_max / 2 - 3, y_max / 2 + 2);
    printf("*************");
    delay(2);
    gotoxy(0, y_max + 1);
    system("/bin/stty cooked");
    return 0;
}
void regen(int mover)
{
    injury[mover] -= 2;
    if(injury[mover] < 0)
        injury[mover] = 0;
    else
        return;
    health[mover] += 2;
    if(health[mover] > health_max)
        health[mover] = health_max;
}
