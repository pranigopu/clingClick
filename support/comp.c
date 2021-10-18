char key;
void comp()
{
    int itemheld = 0; 
    int m, n, flag = 1, tmp, plus, i;
    m = pos[P][X] - pos[C][X];
    n = pos[P][Y] - pos[C][Y];
    if(m < 0) //Player to the left
    {
        flag *= 1;
        key = 'a';
    }
    else if(m > 0) //Player to the right
    {
        flag *= 2;
        key = 'd';
    }
    if(n < 0) //Player above
    {
        flag *= 3;
        key = 'w';
    }
    else if(n > 0) //Player below
        key = 's';
    if(m >= -xtick && m <= xtick && n == 0) //Player at melee distance
    {
        flag *= 5;
        itemheld = 1;
    }
    else if(n >= -ytick && n <= ytick && m == 0) //Player at melee distance
    {
        flag *= 5;
        itemheld = 1;
    }
    else if(m == 0) //Player in line of attack
    {
        if(n < 0)
            plus = -ytick;
        else
            plus = ytick;
        for(i = pos[C][Y]; (tmp = detectObstacle(pos[C][X], i, C)) == 0 && i >= y_min && i <= y_max && i != pos[P][Y]; i += plus);
        if(tmp == 0)
        {
            flag *= 5;
            itemheld = 2;
        }
    }
    else if(n == 0) //Player in line of attack
    {
        if(m < 0)
            plus = -xtick;
        else
            plus = xtick;
        for(i = pos[C][X]; (tmp = detectObstacle(i, pos[C][Y], C)) == 0 && i >= x_min && i <= x_max && i != pos[P][X]; i += plus);
        if(tmp == 0)
        {
            flag *= 5;
            itemheld = 2;
        }
    }
    if(flag % 5 != 0)
    {
        gotoxy(pos[C][X], pos[C][Y]);
        printf(" ");
        move();
    }
    else
        act(key, itemheld, citem, C);
}
void move() 
{
    void approach();
    void escape();
    if(health[C] < health[P])
        escape();
    else
        approach();
}
void approach()
{
    pathElement *ptr;
    ptr = pathFind(pos[C][X], pos[C][Y], pos[P][X], pos[P][Y]);
    if(ptr != NULL)
    {
        while(ptr -> source != all[SET])
        {
            //LINES OF CODE TO SEE THE COMPLETED PATH 
            //gotoxy(ptr -> x, ptr -> y);
            //printf("*");
            
            // NECESSARY LINE
            ptr = ptr -> source;
        }
        delay(0.08);
        pos[C][X] = ptr -> x;
        pos[C][Y] = ptr -> y;
    }
    freeLists();
}
void escape()
{
    int danger(int, int);
    int i, x, y;
    x = pos[C][X];
    y = pos[C][Y];
    if(danger(x, y) == 0)
        return;
    for(i = -xtick; i <= xtick; i += 2 * xtick)
        if(danger(x + i, y) == 0 && detectObstacle(x + i, y, C) == 0)
        {
            if(x + i >= x_min && x + i <= x_max)
            {
                pos[C][X] = x + i;
                return;
            }
        }
    for(i = -1; i <= 1; i += 2)
        if(danger(x, y + i) == 0 && detectObstacle(x, y + i, C) == 0)
        {
            if(y + i >= y_min && y + i <= y_max)
            {
                pos[C][Y] = y + i;
                return;
            }
        }
}
int danger(int x, int y)
{
    if(health[P] <= health[C])
        return 0;
    if(inMeleeDist(x, y, pos[P][X], pos[P][Y])) //Player at melee distance
            return 1;
    if(x == pos[P][X] || y == pos[P][Y]) //Player in line of attack
        return 1;
    return 0;
}