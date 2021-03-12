typedef struct list
{
    int x;
    int y;
    struct list *next; 
    struct list *prev;
} way;
//Variables global for pathfinder.c and comp.c
way *open[2] = {NULL, NULL};
way *shut[2] = {NULL, NULL};
int count = 0;
int limit = 0;

//Function declarations
void switchval(int *, int *);
void prioritise(int, int, int, int, int *);
int check(int, int, way *[]);
int search(int, int, way *);
void insert(int, int, way *[]);
void delete(int, int, way *[]);
void trim(way *[]);
void replace(way *[]);
void freelist(way *[]);

//Function definitions
void switchval(int *a, int *b)
{
    int tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}
void prioritise(int x_start, int x_end, int y_start, int y_end, int *order)
{
    int x, y, i, j;
    //Prioritising by direction
    if(x_start > x_end)
    {
        order[0] = 2;
        order[2] = 1;
        x = x_start - 1 - x_end;
        i = x_start + 1 - x_end;    
    }
    else
    {
        order[0] = 1;
        order[2] = 2;
        x = x_start + 1 - x_end;
        i = x_start - 1 - x_end;
    }
    if(y_start > y_end)
    {
        order[1] = 4;
        order[3] = 3;
        y = y_start - 1 - y_end;
        j = y_start + 1 - y_end;
    }
    else
    {
        order[1] = 3;
        order[3] = 4;
        y = y_start + 1 - y_end;
        j = y_start - 1 - y_end;
    }
    //Prioritising by displacement
    x = ytick * x * ytick * x + xtick * (y_start - y_end) * xtick * (y_start - y_end);
    y = xtick * y * xtick * y + ytick * (x_start - x_end) * ytick * (x_start - x_end);
    i = ytick * i * ytick * i + xtick * (y_start - y_end) * xtick * (y_start - y_end);
    j = xtick * j * xtick * j + ytick * (x_start - x_end) * ytick * (x_start - x_end);
    if(y < x)
        switchval(&order[0], &order[1]);
    if(j < i)
        switchval(&order[2], &order[3]);
}
int check(int x, int y, way *w[])
{
    if(x > x_max || x < x_min)
        return 1;
    else if(y > y_max || y < y_min)
        return 2;
    else if(detectObstacle(x, y))
        return 3;
    else if(search(x, y, w[0]))
        return 4;
    else if(search(x, y, shut[0]))
        return 5;
    return 0;
}
int search(int x, int y, way *head)
{
    while(head != NULL)
    {
        if(head -> x == x && head -> y == y)
            return 1;
        head = head -> next;
    }
    return 0;
}
void insert(int x, int y, way *w[])
{
    way * tmp;
    if(w[0] == NULL)
    {
        w[0] = (way *)malloc(sizeof(way));
        w[0] -> prev = NULL;
        w[1] = w[0];
        count = 1;
    }
    else
    {
        
        tmp = w[1];
        w[1] -> next = (way *)malloc(sizeof(way)); 
        w[1] = w[1] -> next;
        w[1] -> prev = tmp; 
    }
    w[1] -> next = NULL;
    w[1] -> x = x;
    w[1] -> y = y;
    ++count;
}
void delete(int x, int y, way *w[])
{ 
    if(w[0] == NULL || w[1] == NULL)
        return;
    way *tmp, *head;
    if(w[1] -> x == x && w[1] -> y == y)
    {
        tmp = w[1];
        w[1] = w[1] -> prev;
        if(w[1] != NULL)
            w[1] -> next = NULL;
    }
    else if(w[0] -> x == x && w[0] -> y == y)
    {
        tmp = w[0];
        w[0] = w[0] -> next;
        w[0] -> prev = NULL;
        free(tmp);
    }
    else
    {
        head = w[0];
        while(head -> next != NULL)
        {
            tmp = head -> next;
            if(tmp -> x == x && tmp -> y == y)
                break;
            head = head -> next;
        }
        if(head -> next != NULL)
        {
            tmp = head -> next;
            head -> next = tmp -> next;
            if(tmp -> next != NULL)
                tmp -> next -> prev = head;  
            free(tmp);      
        }
    }
    --count; 
}
void trim(way *w[])
{
    /*
    NOTES ON trim
    This function shortens the path of the computer character.
    The function does the following:
    1. Compares current node's position to other nodes' positions
    2. Identifies the second node whose position is in melee distance from the current node's positions
       (This means there is a chain of extra nodes where there could be a smaller chain)
    3. Deletes all nodes from the node preceding the current node to the node succeeding the identified node
       (This directly links the identified node to the current node, hence shortening the path)
    4. Simultaneously inserts deleted positions into the 'shut' list so that the positions won't be added again
    */
    int i;
    way *tmp;
    for(i = 0; i < 3; ++i)
    {
        tmp = w[1] -> prev;
        while(tmp != NULL && tmp -> prev != NULL)
        {
            tmp = tmp -> prev;
            if(inMeleeDist(w[1] -> x, w[1] -> y, tmp -> x, tmp -> y)) //Step 1 and 2
            {
                //Step 3
                tmp = tmp -> next;
                while(tmp != w[1])
                {
                    insert(tmp -> x, tmp -> y, shut);
                    delete(tmp -> x, tmp -> y, w);
                    tmp = tmp -> next;
                }
                break;
            }
        }  
    } 
}
void replace(way *w[])
{
    /*
    NOTES ON replace
    When a new way is found but some of the positions constituting the way are in the 'shut' list,
    this function removes these positions from the 'shut' list and replaces them to the 'open' list. 
    */
    if(w[1] == NULL)
        return;
    int order[4], x, y, i, j;
    way cur;
    cur.x = w[1] -> x;
    cur.y = w[1] -> y;
    if(w[1] -> prev != NULL && !inMeleeDist(cur.x, cur.y, w[1] -> prev -> x, w[1] -> prev -> y))
    {
        delete(w[1] -> x, w[1] -> y, w);
        do
        {
            prioritise(w[1] -> x, cur.x, w[1] -> y, cur.y, order);
            for(i = 0; i < 4; i++)
            {
                x = w[1] -> x;
                y = w[1] -> y;
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
                if((j = check(x, y, w)) == 5 || j == 0)
                {
                    delete(x, y, shut);
                    insert(x, y, w);
                    trim(w);
                    break;
                }
            }
        } while(w[1] -> x != cur.x || w[1] -> y != cur.y);
    }
}
void freelist(way *w[])
{
    way *tmp;
    while(w[0] != NULL)
    {
        tmp = w[0];
        w[0] = w[0] -> next;
        free(tmp);
    }
    w[1] = NULL;
    
}