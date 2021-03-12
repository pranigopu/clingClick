int areSame(char *str1, char *str2)
{
    int i = 0;
    while(str1[i] != '\0' && str2[i] != '\0')
    {
        if(str1[i] != str2[i])
            return 0;
        i++;
    }
    if(str1[i] != '\0' || str2[i] != '\0')
        return 0;
    return 1;
}
void assign(char *str1, char *str2)
{
    int i = 0;
    while(str1[i] != '\0')
    {
        str2[i] = str1[i];
        i++;
    }
    str2[i] = '\0';
}
void addItem(char *cn, char *cs, int type, int dmg)
{
    if(inventory_count >= items_max)
        return;
    int i;
    for(i = 0; i < inventory_count; i++)
        if(areSame(cn, pitem[i].name))
        {
            if(pitem[i].count < 4)
                pitem[i].count++;
            return;
        }
    assign(cn, pitem[inventory_count].name);
    assign(cs, pitem[inventory_count].symbol);
    pitem[inventory_count].type = type;
    pitem[inventory_count].dmg = dmg;
    pitem[inventory_count].count = 1;
    inventory_count++;
}
void removeItem(iteminfo *item, int itemheld)
{
    int i;
    for(i = itemheld + 1; i < inventory_count; i++)
    {
        assign(item[i - 1].name, item[i].name);
        assign(item[i - 1].symbol, item[i].symbol);
        item[i - 1].type = item[i].type;
        item[i - 1].dmg = item[i].dmg;
        item[i - 1].count = item[i].count;
    }
    inventory_count--;
}
char seek(FILE *fp, int x, int y)
{
    int xcount = 0, ycount = 0;
    char c;
    fseek(fp, 0, SEEK_SET);
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
    fseek(fp, -1, SEEK_CUR);
    return c;
}
