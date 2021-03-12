#include "pathfinderSupport.c"

//Global variables
extern way *open1[2];
extern way *open2[2];
extern way *shut[2];
extern int count;

//Function declarations
extern void switchval(int *, int *);
extern int check(int, int, way *[]);
extern int search(int, int, way *);
extern void insert(int, int, way *[]);
extern void delete(int, int, way *[]);
extern void trim(way *[]);
extern void freelist(way *[]);

//Pathfinding function
int pathfind(int x_start, int y_start, int x_end, int y_end, way *w[])
{    
    
    //LINES OF CODE TO SEE THE PATH BEING CREATED
    way *tmp;
    tmp = w[0];  
    system("clear");
    console(0);    
    while(tmp != NULL)
    {
        gotoxy(tmp -> x, tmp -> y);
        printf("*");
        tmp = tmp -> next;
    }
    fseek(stdin, 0, 0);
    getchar();
    
    int x, y, i;
    int order[4];
    //Establishing the order in which the positions must be checked
    prioritise(x_start, x_end, y_start, y_end, order);
    
    /*
    CONDITIONS TO AVOID
    1.  Next position is in closed list
    2.  Next position is in open list
    3.  Next position exceeds max_x or max_y
    4.  Next position is below min_x or min_y
    5.  Next position contains obstacle
    
    If all three next positions for the current position are invalid, 
    put current position in shut list.
    */

    insert(x_start, y_start, w);
    trim(w);
    replace(w);
    for(i = 0; i < 4; i++)
    {
        x = x_start;
        y = y_start;
        switch(order[i])
        {
            case 1:
                x += xtick;
                break;
            case 2:
                x -= xtick;
                break;
            case 3:
                y += ytick;
                break;
            case 4:
                y -= ytick;
                break;
        }
        if(check(x, y, w) == 0)
        {
            if(x == x_end && y == y_end)
            {
                insert(x, y, w);
                return 1;
            }
            else if(pathfind(x, y, x_end, y_end, w))
                return 1;
        }
    }
    if(w[0] -> x != x_start || w[0] -> y != y_start)
    {
        insert(x_start, y_start, shut); 
        delete(x_start, y_start, w);  
    }
    return 0; 
}
