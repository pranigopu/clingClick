char *concatenate(char *a, char *b)
{
    int i, j;
    char *str;
    for(i = 0; a[i] != '\0'; i++);
    for(j = 0; b[j] != '\0'; i++, j++);
    str = (char *)malloc(sizeof(char) * (i + 1));
    for(i = 0; a[i] != '\0'; i++)
        str[i] = a[i];
    for(j = 0; b[j] != '\0'; i++, j++)
        str[i] = b[j];
    str[i] = '\0';
    return str;
}
char *intAsString(int n)
{
    int tmp = n, digits = 0, i;
    char *str, c;
    while(tmp != 0)
    {
        tmp /= 10;
        digits++;
    }
    str = (char *)malloc(sizeof(char) * (digits + 1));
    str[digits] = '\0';
    for(i = digits - 1; i >= 0; i--)
    {
        c = n % 10 + '0';
        n /= 10;
        str[i] = c;
    }
    return str;
}
void updateMap(int mapNo, int mover)
{
    char *mapName;
    int i;
    mapName = getMapName(mapNo, 0);
    for(i = 0; i < map_name_max_chars - 1 && mapName[i] != '\0'; i++)
        currentMap[mover][i] = mapName[i];
    currentMap[mover][i] = '\0';
    free(mapName);

    mapName = getMapName(mapNo, 1);
    for(i = 0; i < map_name_max_chars - 1 && mapName[i] != '\0'; i++)
        currentMapValues[mover][i] = mapName[i];
    currentMapValues[mover][i] = '\0';
    free(mapName);
}
char *getMapName(int mapNo, int isValueFile)
{
    char *mapName, *tmp;
    int i;
    tmp = intAsString(mapNo);
    mapName = concatenate("gameMaps/m", tmp);
    free(tmp);
    tmp = mapName;
    if(isValueFile)
        mapName = concatenate(tmp, "_values.txt");
    else
        mapName = concatenate(tmp, ".txt");
    return mapName;
}
int gotoMapNoAt(int targetRow, int targetCol)
{
    if(targetRow < 1 || targetCol < 1)
        return 0;
    FILE *map_layout = fopen("_map_layout.txt", "r");
    char c = ' ';
    int mapNo = 0, row = 1, col = 0;
    fseek(map_layout, 0, SEEK_SET);
    //Seeking the target row
    while(row < targetRow && c != EOF)
    {
        while((c = fgetc(map_layout)) != '\n' && c != EOF);
        row++;
    }
    //Seeking the target column
    while(col < targetCol && c != EOF)
    {
        mapNo = 0;
        if(c == ' ' || c == '\t' || c == '\n')
            while((c = fgetc(map_layout)) == ' ' && c == '\t');
        if(c >= '0' && c <= '9')
        {
            if(++col < targetCol)
                while((c = fgetc(map_layout)) >= '0' && c <= '9');
            else
            {
                mapNo = c - '0';
                while((c = fgetc(map_layout)) >= '0' && c <= '9')
                {
                    mapNo *= 10;
                    mapNo += c - '0';
                }
                break;
            }
        }
    }
    fclose(map_layout);
    return mapNo;
}
void insertMapNo(int mapNo, int row, int col, map *ptr[])
{
    if(ptr[SET] == NULL || ptr[CUR] == NULL)
    {
        ptr[SET] = (map *)malloc(sizeof(map));
        ptr[CUR] = ptr[SET];
    }
    else
    {
        ptr[CUR] -> link = (map *)malloc(sizeof(map));
        ptr[CUR] = ptr[CUR] -> link;
    }
    ptr[CUR] -> mapNo = mapNo;
    ptr[CUR] -> n = gotoMapNoAt(row - 1, col);
    ptr[CUR] -> s = gotoMapNoAt(row + 1, col);
    ptr[CUR] -> w = gotoMapNoAt(row, col - 1);
    ptr[CUR] -> e = gotoMapNoAt(row, col + 1);
    printf("%d, %d\n", row, col);
    printf("%d, %d, %d, %d\n\n", ptr[CUR] -> n, ptr[CUR] -> s, ptr[CUR] -> w, ptr[CUR] -> e);
    fseek(stdin, 0, 0);
    getchar();
    ptr[CUR] -> link = NULL;
}
int loadMaps()
{
    FILE *gameMap, *gameMapValues, *savedMap, *map_list;
    char *gameMapName, savedMapName[100], c = ' ', t = ' ';
    int xcount, ycount, i, mapCount = 1;
    map_list = fopen("support/_map_list.txt", "r");
    while((c = fgetc(map_list)) != EOF)
    {
        fseek(map_list, -1, SEEK_CUR);
        gameMapName = getMapName(mapCount, 0);
        gameMap = fopen(gameMapName, "w");
        
        /*
        Previous error...
        Before, I had done the following code:
        tmp = mapName;

        The issue with this is that mapName is a pointer. Assigning tmp to mapName's value assigns the location of the value pointer by mapName to tmp as well.
        Hence, any change done through mapName on the value will be reflected on tmp, and vice versa.
        However, the purpose of tmp was to hold the first value of mapName.
        */
        
        fscanf(map_list, "%s", savedMapName);
        if((savedMap = fopen(savedMapName, "r")) == NULL)
        {
            printf("The file %s is not present in the directory support/savedMaps/\n", savedMapName);
            continue;
        }
        else
            printf("The map in the file %s will be loaded :)\n", savedMapName);
        //Copying saved maps to game maps
        xcount = x_min - 1;
        ycount = y_min;
        fseek(savedMap, 0, SEEK_SET);
        while((c = fgetc(savedMap)) != EOF)
        {
            xcount++;
            //Making spaces until the border
            if(c == '\n')
            {
                for(i = xcount; i < x_max; i++)
                    fputc(' ', gameMap);
                fputc('\n', gameMap);
                ycount++;
                xcount = x_min - 1;
            }
            else
                fputc(c, gameMap);
        }
        //Filling empty space with spaces
        while(ycount++ < y_max)
        {
            for(i = x_min; i < x_max; i++)
                fputc(' ', gameMap);
            fputc('\n', gameMap);
        }
        //Committing changes in gameMap and closing savedMap
        fclose(gameMap);
        fclose(savedMap);
        //------------------------
        //ENTERING BLOCK DURABILITIES
        /*
        Note:
        This code must copy the spaces from the game map.
        Not doing so may prevent accurate updation of the maps when blocks are placed (beyond the end of the line of obstacles).
        */
        gameMap = fopen(gameMapName, "r");
        gameMapName = getMapName(mapCount, 1);
        gameMapValues = fopen(gameMapName, "w");
        //free(gameMapName);

        xcount = -1;
        ycount = 0;
        fseek(gameMap, 0, SEEK_SET);
        while((c = fgetc(gameMap)) != EOF)
        {
            xcount++;
            i = isObstacle(c);
            if(c == '\n')
            {
                fputc('\n', gameMapValues);
                ycount++;
                xcount = -1;
            }
            else if(i)
                fprintf(gameMapValues, "%d", block[i - 1].durability);
            else
                fputc(' ', gameMapValues);
        }
        fclose(gameMap);
        fclose(gameMapValues);
        mapCount++;
    }
    if(mapCount == 0)
    {
        printf("There are no available maps to play in :(\n");
        return 0;
    }
    currentMapNo[P] = 1;
    currentMapNo[C] = 1;
    updateMap(currentMapNo[P], P);
    updateMap(currentMapNo[C], C);
    printf("Enter to continue...");
    fseek(stdin, 0, SEEK_SET);
    getchar();
    return 1;
}
int loadMapLayout()
{
    FILE *map_layout = fopen("_map_layout.txt", "r");
    char c = ' ';
    int mapNo, row = 1, col = 0;
    while((c = fgetc(map_layout)) != EOF)
    {
        if(c == '\n')
        {
            col = 0;
            row++;
        }
        else if(c >= '0' && c <= '9')
        {
            col++;
            mapNo = c - '0';
            while((c = fgetc(map_layout)) >= '0' && c <= '9')
            {
                mapNo *= 10;
                mapNo += c - '0';
            }
            if(c != EOF)
                fseek(map_layout, -1, SEEK_CUR);
            insertMapNo(mapNo, row, col, gameMap);
        }
    }
    fclose(map_layout);
    return 0;
}