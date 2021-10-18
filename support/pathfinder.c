void insertPathElement(int x, int y, pathElement *source, pathElement *ptr[])
{
    if(ptr[SET] == NULL || ptr[CUR] == NULL)
    {
        ptr[SET] = (pathElement *)malloc(sizeof(pathElement));
        ptr[SET] -> prev = NULL;
        ptr[CUR] = ptr[SET];
    }
    else
    {
        ptr[CUR] -> link = (pathElement *)malloc(sizeof(pathElement));
        ptr[CUR] -> link -> prev = ptr[CUR];
        ptr[CUR] = ptr[CUR] -> link;
    }
    ptr[CUR] -> link = NULL;
    ptr[CUR] -> x = x;
    ptr[CUR] -> y = y;
    ptr[CUR] -> h = (pos[P][X] - x) * (pos[P][X] - x) + (pos[P][Y] - y) * (pos[P][Y] - y);
    ptr[CUR] -> g = (pos[C][X] - x) * (pos[C][X] - x) + (pos[C][Y] - y) * (pos[C][Y] - y);
    ptr[CUR] -> source = source;
}
void insertCoord(int x, int y, coord *ptr[])
{
    if(ptr[SET] == NULL || ptr[CUR] == NULL)
    {
        ptr[SET] = (coord *)malloc(sizeof(coord));
        ptr[CUR] = ptr[SET];
    }
    else
    {
        ptr[CUR] -> link = (coord *)malloc(sizeof(coord));
        ptr[CUR] = ptr[CUR] -> link;
    }
    ptr[CUR] -> link = NULL;
    ptr[CUR] -> x = x;
    ptr[CUR] -> y = y;
}
void deletePathElement(int x, int y, pathElement *ptr[])
{
    pathElement *tmp = ptr[SET];
    while(tmp != NULL)
    {
        if(tmp -> x == x && tmp -> y == y)
            break;
        tmp = tmp -> link;
    }
    if(tmp == NULL)
        return;
    if(tmp -> prev == NULL)
    {
        ptr[SET] = ptr[SET] -> link;
        ptr[SET] -> prev = NULL;
        free(tmp);
    }
    else
    {
        tmp -> prev -> link = tmp -> link;
        if(tmp -> link == NULL)
        {
            ptr[CUR] = ptr[CUR] -> prev;
            ptr[CUR] -> link = NULL;
        }
        else
        {
            tmp -> link -> prev = tmp -> prev;
        }
        free(tmp);
    }
}
int searchCoord(int x, int y, coord *ptr)
{
    while(ptr != NULL)
    {
        if(ptr -> x == x && ptr -> y == y)
            return 1;
        ptr = ptr -> link;
    }
    return 0;
}
int check(int x, int y)
{
    if(x < x_min || x > x_max)
        return 1;
    if(y < y_min || y > y_max)
        return 2;
    if(detectObstacle(x, y, C))
        return 3;
    if(searchCoord(x, y, blocked[SET]) || searchCoord(x, y, visited[SET]))
        return 4;
    return 0;
}
int branch(pathElement *ptr)
{
    int x, y, i, successful_inserts = 0;
    pathElement *tmp;
    for(i = 0; i < 4; i++)
    {
        x = ptr -> x;
        y = ptr -> y;
        switch(i)
        {
            case 0:
                x += xtick;
                break;
            case 1:
                x -= xtick;
                break;
            case 2:
                y += ytick;
                break;
            case 3:
                y -= ytick;
                break;
        }
        if(check(x, y) == 0)
        {
            successful_inserts++;
            insertPathElement(x, y, ptr, all);
        }
    }
    if(successful_inserts == 0)
    {
        insertCoord(ptr -> x, ptr -> y, blocked);
        //deletePathElement(ptr -> x, ptr -> y, all);
        return 0;
    }
    return 1;
}
//Function to estimate the next step in the most efficient path to the player...
pathElement *minimum(int x_end, int y_end)
{
    pathElement *min, *ptr;
    ptr = all[SET];
    while(searchCoord(ptr -> x, ptr -> y, blocked[SET]) || searchCoord(ptr -> x, ptr -> y, visited[SET]))
        ptr = ptr -> link;
    min = ptr;
    ptr = ptr -> link;
    while(ptr != NULL)
    {
        if(searchCoord(ptr -> x, ptr -> y, blocked[SET]) || searchCoord(ptr -> x, ptr -> y, visited[SET]))
        {
            ptr = ptr -> link;
            continue;
        }
        else if(ptr -> x == x_end && ptr -> y == y_end)
            return ptr;
        else if(ptr -> h < min -> h)
            min = ptr;
        else if(ptr -> h == min -> h && ptr -> g < min -> g)
            min = ptr;
        ptr = ptr -> link;
    }
    return min;
}
void freePathElementList(pathElement *ptr[])
{
    pathElement *tmp;
    while(ptr[SET] != NULL)
    {
        tmp = ptr[SET];
        ptr[SET] = ptr[SET] -> link;
        free(tmp);
    }
    ptr[CUR] = NULL;
}
void freeCoordList(coord *ptr[])
{
    coord *tmp;
    while(ptr[SET] != NULL)
    {
        tmp = ptr[SET];
        ptr[SET] = ptr[SET] -> link;
        free(tmp);
    }
    ptr[CUR] = NULL;
}
void freeLists()
{
    freePathElementList(all);
    freeCoordList(blocked);
    freeCoordList(visited);
}
pathElement *pathFind(int x_start, int y_start, int x_end, int y_end)
{
    insertPathElement(x_start, y_start, NULL, all);
    int x, y, i = 0;
    pathElement *min, *tmp = NULL;
    min = all[SET];
    do
    {
        // Checking if min is NULL.
        // If the computer's routes are completely blocked, min will eventually become NULL.
        if(min == NULL)
            return NULL;

        // Retracing steps if necessary.
        if(branch(min) == 0)
        {
            /*
            NOTE ON RETRACING STEPS
            If no branching steps can be taken from the current position, that means the current position is blocked from all sides except the previous step.
            Hence, you backtrack to the previous step and try new branches (having put that deadend position into the blocked list).
            */
            min = min -> prev;
            continue;
        }
        min = minimum(x_end, y_end);
        if(min == NULL)
            return NULL;
        insertCoord(min -> x, min -> y, visited);
        
        /*
        //------------------------
        //LINES OF CODE TO SEE THE ALGORITHM IN PROCESS
        coord *show;
        show = visited[SET];
        while(show != NULL)
        {
            gotoxy(show -> x, show -> y);
            printf("*");
            show = show -> link;
        }
        fseek(stdin, 0, 0);
        getchar();
        //------------------------
        */

        if(health[C] > health[P])
        {
            if(min -> x == x_end && min -> y == y_end)
                return min;
        }
        else if(inMeleeDist(x_end, y_end, min -> x, min -> y))
            return tmp;
        if(min -> x == x_end && min -> y == y_end)
            return min;
        tmp = min;
    } while(++i < 300);
    return NULL;
}