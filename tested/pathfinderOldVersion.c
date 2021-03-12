#define CUR 1
#define SET 0
typedef struct list
{
    int x;
    int y;
    int g;
    int h;
    struct list *link;
    struct list *prev;
    struct list *source;
} node;
node *all[2] = {NULL, NULL};
node *open[2] = {NULL, NULL};
node *shut[2] = {NULL, NULL};
void insert(int x, int y, node *source, node *ptr[])
{
    if(ptr[SET] == NULL || ptr[CUR] == NULL)
    {
        ptr[SET] = (node *)malloc(sizeof(node));
        ptr[SET] -> prev = NULL;
        ptr[CUR] = ptr[SET];
    }
    else
    {
        ptr[CUR] -> link = (node *)malloc(sizeof(node));
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
void delete(int x, int y, node *ptr[])
{
    node *tmp = ptr[CUR];
    while(tmp != NULL && (tmp -> x != x || tmp -> y != y))
        tmp = tmp -> prev;
    if(tmp == NULL)
        return;
    if(tmp -> prev != NULL)
        tmp -> prev -> link = tmp -> link;
    else
    {
        ptr[SET] = ptr[SET] -> link;
        if(ptr[SET] != NULL)
            ptr[SET] -> prev = NULL;
    }
    if(tmp -> link != NULL)
        tmp -> link -> prev = tmp -> prev;
    else
    {
        ptr[CUR] = ptr[CUR] -> prev;
        if(ptr[CUR] != NULL)
            ptr[CUR] -> link = NULL;
    }
}
int search(int x, int y, node *ptr)
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
    if(detectObstacle(x, y))
        return 3;
    if(search(x, y, shut[SET]))
        return 4;
    return 0;
}
void branch(node *ptr)
{
    int x, y, i, successful_inserts = 0;
    node *tmp;
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
            insert(x, y, ptr, all);
            insert(x, y, ptr, open);
        }
    }
    if(successful_inserts == 0)
    {
        insert(ptr -> x, ptr -> y, NULL, shut);
        delete(ptr -> x, ptr -> y, open);
    }
}

node *minimum(int x_end, int y_end)
{
    node *min, *ptr;
    ptr = all[SET];
    while(search(ptr -> x, ptr -> y, shut[SET]))
        ptr = ptr -> link;
    min = ptr;
    ptr = ptr -> link;
    while(ptr != NULL)
    {
        if(ptr -> x == x_end && ptr -> y == y_end)
            return ptr;
        if(search(ptr -> x, ptr -> y, shut[SET]))
        {
            ptr = ptr -> link;
            continue;
        }
        if(ptr -> h < min -> h)
            min = ptr;
        else if(ptr -> h == min -> h)
        {
            if(ptr -> g < min -> g)
                min = ptr;
        }
        ptr = ptr -> link;
    }
    return min;
}
void freeList(node *ptr[])
{
    node *tmp;
    while(ptr[SET] != NULL)
    {
        tmp = ptr[SET];
        ptr[SET] = ptr[SET] -> link;
        free(tmp);
    }
    ptr[CUR] = NULL;
}
node *pathFind(int x_start, int y_start, int x_end, int y_end)
{
    insert(x_start, y_start, NULL, all);
    insert(x_start, y_start, NULL, open);
    int x, y, i = 0;
    node *min, *tmp = NULL, *show;
    min = all[SET];
    do
    {
        insert(min -> x, min -> y, NULL, shut);
        delete(min -> x, min -> y, open);
        branch(min);
        min = minimum(x_end, y_end);
        /*
        //LINES OF CODE TO SEE THE ALGORITHM IN PROCESS
        
        show = open[SET];
        while(show != NULL)
        {
            gotoxy(show -> x, show -> y);
            printf("*");
            show = show -> link;
        }
        fseek(stdin, 0, 0);
        getchar();
        */
        if(inMeleeDist(x_end, y_end, min -> x, min -> y))
            return tmp;
        if(min -> x == x_end && min -> y == y_end)
            return min;
        tmp = min;
    } while(++i < 300);
    return NULL;
}