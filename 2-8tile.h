#define SIZE 3
#define CELLS 9
#define DELAY 500.0
#define SCREEN_COLS 15
#define SCREEN_ROWS 21
#define WIDTH 5
#define HEIGHT 7

struct node{
   char board[SIZE][SIZE];
   struct node *next;
   struct node *parent;
};
typedef struct node Node;

void printboard(char board[SIZE][SIZE]);
void explore(char board[SIZE][SIZE], int *x, int *y, Node *front, Node *current);
int compare(char board[SIZE][SIZE], char original[SIZE][SIZE]);
void findspace(char board[SIZE][SIZE], int *x, int *y);
void swap(char *a, char *b);
void validboard(char board[SIZE][SIZE]);
int storeboard(Node *front, Node *current);
void copyboard(char board[SIZE][SIZE], char copy[SIZE][SIZE]);
void fileread(FILE *fp, char board[SIZE][SIZE]);
void iterate(int *x, int *y, Node *front);
int printreverse(Node *current, Node *front);
void PrintList(Node *current);
Node *AllocateBoard(char board[SIZE][SIZE]);
void freeboard(Node *front);
void printsolution(Node *current, Node *front);
void copynumber(char number[HEIGHT][WIDTH], char final[SCREEN_ROWS][SCREEN_COLS], int colus, int rows);
void makeboard(char board[SIZE][SIZE], char final[SCREEN_ROWS][SCREEN_COLS]);
void inversions(char string[CELLS+1]);
void solvable(char board[SIZE][SIZE]);

