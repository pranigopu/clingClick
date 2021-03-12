#include <stdio.h>
#include <stdlib.h>
#include "../clclSupport/clclFunctions.h"
#define SET 0
#define CUR 1
typedef struct mapList
{
    int mapNo, n, s, w, e;
    struct mapList *link;
} map;
int mapLayoutRow = 0, mapLayoutCol = 0;
map *gameMap[2] = {NULL, NULL};
void insertMapNo(int, int, int, map *[]);
int loadMapLayout();
int gotoMapNoAt(int, int);
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
int loadMapLayout()
{
    FILE *mapLayout = fopen("mapLayout.txt", "r");
    char c = ' ';
    int mapNo, row = 1, col = 0;
    while((c = fgetc(mapLayout)) != EOF)
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
            while((c = fgetc(mapLayout)) >= '0' && c <= '9')
            {
                mapNo *= 10;
                mapNo += c - '0';
            }
            if(c != EOF)
                fseek(mapLayout, -1, SEEK_CUR);
            insertMapNo(mapNo, row, col, gameMap);
        }
    }
    fclose(mapLayout);
    return 0;
}
int gotoMapNoAt(int targetRow, int targetCol)
{
    if(targetRow < 1 || targetCol < 1)
        return 0;
    FILE *mapLayout = fopen("mapLayout.txt", "r");
    char c = ' ';
    int mapNo = 0, row = 1, col = 0;
    fseek(mapLayout, 0, SEEK_SET);
    //Seeking the target row
    while(row < targetRow && c != EOF)
    {
        while((c = fgetc(mapLayout)) != '\n' && c != EOF);
        row++;
    }
    //Seeking the target column
    while(col < targetCol && c != EOF)
    {
        mapNo = 0;
        if(c == ' ' || c == '\t' || c == '\n')
            while((c = fgetc(mapLayout)) == ' ' && c == '\t');
        if(c >= '0' && c <= '9')
        {
            if(++col < targetCol)
                while((c = fgetc(mapLayout)) >= '0' && c <= '9');
            else
            {
                mapNo = c - '0';
                while((c = fgetc(mapLayout)) >= '0' && c <= '9')
                {
                    mapNo *= 10;
                    mapNo += c - '0';
                }
                break;
            }
        }
    }
    fclose(mapLayout);
    return mapNo;
}
void readFile(char *fileName)
{
    FILE *fp;
    char c;
    fp = fopen(fileName, "r");
    while((c = fgetc(fp)) != EOF)
        putchar(c);
}
/*
To use this code, include clclFunctions.h from clclSupport/.
Also, make sure to put this source code in the clclSupport/ directory to avoid errors.
*/
int main()
{
    loadMapLayout();
    FILE *fp;
    map *x;
    int tmp;
    char direction;
    updateMap(1);
    readFile(currentMap);
    gameMap[CUR] = gameMap[SET];
    do
    {
        printf("Enter direction: ");
        fseek(stdin, 0, SEEK_SET);
        direction = getchar();
        switch(direction)
        {
            case 'n':
                tmp = gameMap[CUR] -> n;
                break;
            case 's':
                tmp = gameMap[CUR] -> s;
                break;
            case 'w':
                tmp = gameMap[CUR] -> w;
                break;
            case 'e':
                tmp = gameMap[CUR] -> e;
                break;
        }
        x = gameMap[SET];
        while(x != NULL)
        {
            if(x -> mapNo == tmp)
                break;
            x = x -> link;
        }
        if(x != NULL)
        {
            gameMap[CUR] = x;
            updateMap(gameMap[CUR] -> mapNo);
        }
        readFile(currentMap);
        printf("\nMap no.:%d\n\n", gameMap[CUR] -> mapNo);

    } while(direction != 'x');
    return 0;
}