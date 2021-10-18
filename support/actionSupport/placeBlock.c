int placeBlock(int mover, int xplus, int yplus, iteminfo *item, int itemheld, int symbol)
{
    FILE *fp1, *fp2;
    int x = pos[mover][X] + xplus;
    int y = pos[mover][Y] + yplus;
    fp1 = fopen(currentMap[mover], "r+");
    fp2 = fopen(currentMapValues[mover], "r+");
    seek(fp1, x - x_min, y - y_min);
    seek(fp2, x - x_min, y - y_min);
    if(item[itemheld].count > 0)
    {
        fputc(item[itemheld].symbol[symbol], fp1);
        fputc(block[isObstacle(item[itemheld].symbol[symbol]) - 1].durability + '0', fp2);
        item[itemheld].count--;
    }
    fclose(fp1);
    fclose(fp2);
    if(item[itemheld].count <= 0)
    {
        removeItem(item, itemheld);
        return itemheld - 1;
    }
    return itemheld;
}