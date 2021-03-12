#include <stdio.h>
#include <stdlib.h>
void gotoxy(int x, int y)    
{
    printf("%c[%d;%df",27,y,x);
}
int detectobstacle(int x, int y)
{
    int xcount = 0, ycount = 0, i;
    char c;
    FILE *fp;
    fp = fopen("test.txt", "r");
    while((c = fgetc(fp)) != EOF && (ycount < y - y_min - 1 || xcount < x - x_min))
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
    printf("Char: %c, xcount: %d, ycount: %d, x: %d, y: %d", c, xcount, ycount, x, y);
    return 0;
}

int main()
{
    int x, y, i;
    FILE *fp;
    while(1)
    {
        char c;
        system("clear");
        printf(">> ");
        scanf("%d%d", &x, &y);
        fp = fopen("test.txt", "r");
        fseek(fp, 10, SEEK_SET);
        detectobstacle(x, y);  
        fseek(stdin, 0, SEEK_SET);
        fclose(fp);
        getchar();
    }
}
