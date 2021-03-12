#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define y_min 10
#define y_max 30
#define x_min 2
#define x_max 60
#define xtick 1
#define ytick 1

#define health_max 20

#define total_items 3
#define items_file
#define items_max 6

#define P 0
#define C 1

#define X 0
#define Y 1

#define SET 0
#define CUR 1

#define map_name_max_chars 30

//GLOBAL STRUCTURES
//Item information
typedef struct _iteminfo
{
    char name[20];
    char symbol[20];
    int type;
    int dmg;
    int count;
} iteminfo;

//Block information
typedef struct _blockinfo
{
    char symbol[2];
    int durability;
    int type;
} blockinfo;

//Map layout
typedef struct mapList
{
    int mapNo, n, s, w, e;
    struct mapList *link;
} map;

//Path construction
typedef struct pathElementList
{
    int x;
    int y;
    int g;
    int h;
    struct pathElementList *link;
    struct pathElementList *prev;
    struct pathElementList *source;
} pathElement;

//Flexible coordinate storage
typedef struct coordList
{
    int x;
    int y;
    struct coordList *link;
} coord;

//GLOBAL VARIABLES
//Mapping
map *gameMap[2] = {NULL, NULL};
int currentMapNo[2];
char currentMap[2][map_name_max_chars];
char currentMapValues[2][map_name_max_chars];

//Pathfinding
pathElement *all[2] = {NULL, NULL};
coord *blocked[2] = {NULL, NULL};
coord *visited[2] = {NULL, NULL};

//Itemhandling
iteminfo pitem[items_max];
iteminfo citem[3];
blockinfo block[3]; 
int itemheld = 0; //Player's item only
int inventory_count = 3;

//Health and injury
int health[2] = {health_max, health_max};
int injury[2] = {0, 0};
short dmg = 0; //Damage counter

//Positions
int pos[2][2] = {13, 11, 5, y_max - 11};

//Predefined options
char projectile[3][5] = {"oooo", "xxxx", "||--"};

//FUNCTIONS
//essentials.c
extern int load();
extern void loadItems();
extern void loadBlockInfo();
extern void gotoxy(int, int);
extern void delay(float);
extern void playArea();
extern void console(int);
extern int isObstacle(char);
extern int detectObstacle(int, int, int);
extern int inMeleeDist(int, int, int, int);

//mapHandling.c
extern char *concatenate(char *,char *);
extern char *intAsString(int);
extern void updateMap(int, int);
extern char *getMapName(int,int);
extern int gotoMapNoAt(int, int);
extern void insertMapNo(int, int, int, map *[]);
extern int loadMaps();
extern int loadMapLayout();

//action.c
extern int act(char, int, iteminfo*, int);

//actionSupport/itemHandling.c
extern int areSame(char *, char *);
extern void assign(char *, char *);
extern void addItem(char *, char *, int, int);
extern void removeItem(iteminfo *, int);
extern char seek(FILE *, int, int);

//actionSupport/mine.c
extern void mine(int, int, int, iteminfo *, int, int);
extern int hit(int, int, int);

//actionSupport/placeBlock.c
extern int placeBlock(int, int, int, iteminfo *, int, int);

//actionSupport/shot.c
extern void shot(int, int, int, iteminfo *, int, int);

//player.c
extern int player();
extern int arrowInput();
extern int inventoryControl(int);

//comp.c
extern void comp();
extern void move();

//SOURCE CODE FILES INCLUDED
#include "essentials.c"
#include "mapHandling.c"

#include "actionSupport/itemHandling.c"
#include "actionSupport/shot.c"
#include "actionSupport/mine.c"
#include "actionSupport/placeBlock.c"
#include "action.c"

#include "player.c"
#include "pathfinder.c"
#include "comp.c"