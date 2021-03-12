int load()
{
    loadItems();
    loadBlockInfo();
    if(loadMaps() == 0)
        return 0;
    return 1;
}
void loadItems()
{
    int i;
    FILE *fp; 
    fp = fopen("clclSupport/player_items.txt", "r");
    fseek(fp, 0, SEEK_SET);
    for(i = 0; i < 3; i++)
    {
        fscanf(fp, "%s", pitem[i].name);
        fscanf(fp, "%s", pitem[i].symbol);
        fscanf(fp, "%d", &pitem[i].type);
        fscanf(fp, "%d", &pitem[i].dmg);
        fscanf(fp, "%d", &pitem[i].count);
    }
    fclose(fp);
    fp = fopen("clclSupport/comp_items.txt", "r");
    fseek(fp, 0, SEEK_SET);
    for(i = 0; i < 3; i++)
    {
        fscanf(fp, "%s", citem[i].name);
        fscanf(fp, "%s", citem[i].symbol);
        fscanf(fp, "%d", &citem[i].type);
        fscanf(fp, "%d", &citem[i].dmg);
    }
    fclose(fp);
}
void loadBlockInfo()
{
    FILE *fp;
    int i;
    fp = fopen("clclSupport/block_info.txt", "r");
    fseek(fp, 0, SEEK_SET);
    for(i = 0; i < 3; i++)
    {
        fscanf(fp, "%s", block[i].symbol);
        fscanf(fp, "%d", &block[i].durability);
        fscanf(fp, "%d", &block[i].type);
    }
    fclose(fp);
}
void gotoxy(int x, int y)    
{
    printf("%c[%d;%df",27,y,x);
}
void delay(float n)
{
    clock_t start, stop;
    int i;
    gotoxy(2, 1);
    printf("\n");
    start = clock();
    while(((double)(stop = clock()) - start) / CLOCKS_PER_SEC <= n);
}
void playArea()
{
    char c;
    FILE *fp;
    int y = y_min, i = x_min;
    gotoxy(x_min, y_min);
    fp = fopen(currentMap[P], "r");
    while((c = fgetc(fp)) != EOF)
    {
        if(c == '\n')
            gotoxy(x_min, ++y);
        else
            putchar(c);
    }
    fclose(fp);
    gotoxy(2, 2);
}
void console(int itemheld)
{
    int i, y = y_min;
    system("clear");
    dmg = 0;
    playArea();

    //Printing inventory status
    gotoxy(2, 2);
    printf("In hand: %s", pitem[itemheld].name);

    //Printing health bars
    gotoxy(2, 3);
    printf("X  ");
    
    for(i = 0; i < health[P]; i++)
        printf("*");
    gotoxy(2, 4);
    printf("O  ");
    for(i = 0; i < health[C]; i++)
        printf("*");

    //Printing player character;
    gotoxy(pos[P][X], pos[P][Y]);
    printf("X");

    //Printing computer character
    gotoxy(pos[C][X], pos[C][Y]);
    printf("O");
}
int isObstacle(char c)
{
    int i;
    for (i = 0; i < 3; i++)
    {
        if(block[i].symbol[0] == c)
            return i + 1;
    }
    return 0;
}
int detectObstacle(int x, int y, int mover)
{
    int xcount = 0, ycount = 0, i;
    char c;
    FILE *fp;
    x = x - x_min;
    y = y - y_min;
    fp = fopen(currentMap[mover], "r");
    while((c = fgetc(fp)) != EOF && (ycount != y || xcount != x))
    {
        if(c == '\n')
        {
            ycount++;
            xcount = 0;
        }
        else
            xcount++;
    }
    fclose(fp);
    if(isObstacle(c))
        return 1;
    return 0;
}
int inMeleeDist(int x, int y, int opp_x, int opp_y)
{

    if(x == opp_x && (y == opp_y - ytick || y == opp_y + ytick))
        return 1;
    if(y == opp_y && (x == opp_x - xtick || x == opp_x + xtick))
        return 1;
    return 0;
}