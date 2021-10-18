int act(char key, int itemheld, iteminfo* item, int mover)
{
    int xplus = 0, yplus = 0, sym = 0, opp;
    char c;
    opp = (mover + 1) % 2;
    switch(key)
    {
        case 'w': 
        xplus = 0;
        yplus = -ytick;
        sym = 0;
        break;

        case 's':
        xplus = 0;
        yplus = ytick;
        sym = 1;
        break;

        case 'd':
        xplus = xtick;
        yplus = 0;
        sym = 2;
        break;

        case 'a':
        xplus = -xtick;
        yplus = 0;
        sym = 3;
        break;

        default:
        return itemheld;
    }
    gotoxy(pos[mover][X] + xplus, pos[mover][Y] + yplus);
    printf("%c", item[itemheld].symbol[sym]);
    if(item[itemheld].type / 10 == 1)
        shot(mover, xplus, yplus, item, itemheld, sym);
    else if(item[itemheld].type / 10 == 2)
        mine(mover, xplus, yplus, item, itemheld, sym);
    else if(item[itemheld].type / 10 == 3)
        itemheld = placeBlock(mover, xplus, yplus, item, itemheld, sym);
    else if(pos[mover][X] + xplus == pos[opp][X] && pos[mover][Y] + yplus == pos[opp][Y])
        dmg = item[itemheld].dmg;
    delay(0.1);
    gotoxy(pos[mover][X] + xplus, pos[mover][Y] + yplus);
    printf(" ");
    return itemheld;
} 