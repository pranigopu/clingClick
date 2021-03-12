int player()
{
    int inventoryControl(int);
    int arrowInput(void);
    char c;
    int i;
    START:
    gotoxy(2, 2);
    switch(c = getchar())
    {
        //Use inventory
        case 'e':
        itemheld = inventoryControl(itemheld);
        console(itemheld);
        goto START;

        //Move
        case 27:
        arrowInput();
        console(itemheld);
        break;

        //Attack
        case 'w': case 's': case 'd': case 'a': case 'g':
        itemheld = act(c, itemheld, pitem, P);
        break;

        //Terminate
        case 'x':
        return -1;;
    }
    return 0;
}
int arrowInput()
{
    char c;
    int x = pos[P][X], y = pos[P][Y];
    int xshift = xtick, yshift = ytick;
    if(getchar() == '[')
    {
        /*
        NOTE:
        A => up
        B => down
        C => right
        D => left
        */
        switch(c = getchar())
        {
            case 'A':
            yshift = -ytick;
            case 'B':
            xshift = 0;
            break;

            case 'D':
            xshift = -xtick;
            case 'C':
            yshift = 0;
            break;
        }
        /*
        if((c = getchar()) == 'A' && y > y_min)
            y -= ytick;
        else if(c == 'B' && y < y_max)
            y += ytick;
        else if(c == 'C' && x < x_max)
            x += xtick;
        else if(c == 'D' && x > x_min)
            x -= xtick;
        */
    }
    else
        return 0;
    x += xshift;
    y += yshift;
    if((x != pos[C][X] || y != pos[C][Y]) && detectObstacle(x, y, P) == 0)
    {
        if(x >= x_min && x <= x_max && y >= y_min && y <= y_max)
        {
            pos[P][X] = x;
            pos[P][Y] = y;
        }
    }
    return 1;
}
int inventoryControl(int itemheld)
{
    int i;
    char c, y = 2 + itemheld;
    system("clear");
    printf("INVENTORY");
    for(i = 0; i < inventory_count; i++)
    {
        gotoxy(2, i + 1);
        printf("\n%s", pitem[i].name);
        if(pitem[i].count > 1)
            printf(" (%d)", pitem[i].count);
    }
    printf("\n");
    gotoxy(2, y);
    while(1)
    {
        fseek(stdin, 0, SEEK_SET);
        if((c = getchar()) == 27)
        {
            if(getchar() == '[')
            {    
                if((c = getchar()) == 'A' && y > 2)
                    gotoxy(2, --y);
                else if(c == 'B' && y < 1 + inventory_count)
                    gotoxy(2, ++y);
            }
        }
        else
            break;
    }
    system("clear");
    return y - 2 ;
}