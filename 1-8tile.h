#define SIZE 3
#define CELLS 9
#define DELAY 500.0
#define SCREEN_SIZE 15
#define START 2
#define SHIFT 5

struct node{
   char board[SIZE][SIZE];
   struct node *next;
   struct node *parent;
};
typedef struct node Node;

void printboard(char board[SIZE][SIZE]);
Node *explore(char board[SIZE][SIZE], int *x, int *y, Node *front, Node *current);
int compare(char board[SIZE][SIZE], char original[SIZE][SIZE]);
void findspace(char board[SIZE][SIZE], int *x, int *y);
void swap(char *a, char *b);
void validboard(char board[SIZE][SIZE]);
int storeboard(Node *front, Node *current);
void copyboard(char board[SIZE][SIZE], char copy[SIZE][SIZE]);
void fileread(FILE *fp, char board[SIZE][SIZE], int *x, int *y);
void iterate(int *x, int *y, Node *front);
int printreverse(Node *current, Node *front);
void PrintList(Node *current);
Node *AllocateBoard(char board[SIZE][SIZE]);
void freeboard(Node *front);
void printsolution(Node *current, Node *front);
void makescreen(char board[SIZE][SIZE], char screen[SCREEN_SIZE][SCREEN_SIZE]);

