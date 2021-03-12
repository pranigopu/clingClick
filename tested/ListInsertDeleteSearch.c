#include <stdio.h>
#include <stdlib.h>
typedef struct list
{
    int x;
    int y;
    struct list *next;
} way;
way *open[2] = {NULL, NULL};
way *shut[2] = {NULL, NULL};
int search(int x, int y, way *head)
{
    while(head != NULL)
    {
        if(head -> x == x && head -> y == y)
            return 1;
        head = head -> next;
    }
    return 0;
}
void insert(int x, int y, way *p[])
{
    if(p[0] == NULL)
    {
        p[0] = (way *)malloc(sizeof(way));
        p[1] = p[0];
    }
    else
    {
        p[1] -> next = (way *)malloc(sizeof(way));
        p[1] = p[1] -> next;
    }
    p[1] -> next = NULL;
    p[1] -> x = x;
    p[1] -> y = y;
}
void delete(int x, int y, way *w[])
{
    way *tmp, *head;
    if(w[0] -> x == x && w[0] -> y == y)
    {
        tmp = w[0];
        w[0] = w[0] -> next;
        free(tmp);
    }
    else
    {
        head = w[0];
        while(head -> next != NULL)
        {
            tmp = head -> next;
            if(tmp -> x == x && tmp -> y == y)
                break;
            head = head -> next;
        }
        if(head -> next != NULL)
        {
            tmp = head -> next;
            head -> next = tmp -> next;
            free(tmp);
        }
    }  
}
void display(way *head)
{
    while(head != NULL)
    {
        printf("\n%d, %d", head -> x, head -> y);
        head = head -> next;
    }
    printf("\n");
}
int main()
{
    int x, y;
    char c;
    while(1)
    {
        printf("\n1: Insert to open\n2: Insert to shut\n3: Delete from open\n4: Delete from shut\n5: Search open\n6: Search shut\nx: Exit\n>> ");
        fseek(stdin, 0, SEEK_SET);
        c = getchar();
        printf("\nx = ");
        scanf("%d", &x);
        printf("y = ");
        scanf("%d", &y);
        switch(c)
        {
            case 'x':
            return 0;

            case '1':
            insert(x, y, open);
            display(open[0]);
            break;

            case '2':
            insert(x, y, shut);
            display(shut[0]);
            break;

            case '3':
            delete(x, y, open);
            display(open[0]);
            break;

            case '4':
            delete(x, y, shut);
            display(shut[0]);
            break;

            case '5':
            printf("\nValue found? %d\n", search(x, y, open[0]));
            break;
        }
    }
}