void mine(int mover, int xplus, int yplus, iteminfo *item, int itemheld, int symbol)
{
    int x, y, opp, i = 0;
    opp = (mover + 1) % 2;
    x = pos[mover][X] + xplus;
    y = pos[mover][Y] + yplus;
    if(detectObstacle(x, y, mover))
        hit(mover, x, y);
    else if(pos[mover][X] + xplus == pos[opp][X] && pos[mover][Y] + yplus == pos[opp][Y])
        dmg = item[itemheld].dmg;
}
int hit(int mover, int x, int y)
{
    FILE *fp;
    char c = 0, cn[2], cs[5];
    int i;
    x = x - x_min;
    y = y - y_min;
    fp = fopen(currentMapValues[mover], "r+");
    fseek(fp, 0, SEEK_SET); 
    c = seek(fp, x, y);
    if(c != '1')
        fputc(c - 1, fp);
    else
        fputc(' ', fp);
    fclose(fp);
    if(c == '1')
    {
        fp = fopen(currentMap[mover], "r+");
        c = seek(fp, x, y);
        cn[0] = c;
        cn[1] = '\0';
        for(i = 0; i < 4; i++)
            cs[i] = c;
        cs[i] = '\0'; 
        if(mover == P)
            addItem(cn, cs, 30, 0);
        fputc(' ', fp);
        fclose(fp);
    }
    return 1;
}