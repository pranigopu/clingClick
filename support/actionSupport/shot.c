void shot(int mover, int xplus, int yplus, iteminfo *item, int itemheld, int symbol)
{
    int x, y, opp, i = 0;
    x = pos[mover][X] + xplus;
    y = pos[mover][Y] + yplus;
    if(detectObstacle(x, y, mover))
        return;
    opp = (mover + 1) % 2;
    while(x + xplus >= x_min && x + xplus <= x_max && y + yplus >= y_min && y + yplus <= y_max)
    {
        if(detectObstacle(x + xplus, y + yplus, mover))
            break;
        gotoxy(x + xplus, y + yplus);
        printf("%c", projectile[item[itemheld].type % 10][symbol]);
        if(x + xplus == pos[opp][X] && y + yplus == pos[opp][Y])
        {
            dmg = item[itemheld].dmg;
            break;
        }
        delay(0.02);
        gotoxy(x + xplus, y + yplus);
        printf(" ");
        x += xplus;
        y += yplus;
    }
}