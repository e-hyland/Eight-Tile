#include <stdio.h>
#include <stdlib.h>
#include "neillncurses.h"
#include <ctype.h>
#include "8tile.h"

/* Solves the 8 Tile game by swapping tiles until the solution board is achieved. Uses queue structure to store boards. */

int main(int argc, char **argv)
{
   FILE *fp;
   Node *front;
   char board[SIZE][SIZE];
   int coorda=0, coordb=0;
   int *x = &coorda, *y = &coordb;

   if (argc == 2){
      if ((fp = fopen(argv[1], "r")) == NULL){
         fprintf(stderr, "Error, please try again. Please enter in valid filename.\n"); 
         return 1;
      } 
      fileread(fp, board, x, y); /* Create board from file, check if valid, and find coordinates of space */
      front = AllocateBoard(board); /* Allocate space to the starting board */
      iterate(x, y, front); /* Swaps tiles and stores to queue */
   }
   else {
      fprintf(stderr, "Error, please enter in format ./dynamic8tile.exe 2.8tile\n");
   }

   return 0;
}

void fileread(FILE *fp, char board[SIZE][SIZE], int *x, int *y)
{
   int i, j=0;
   char c;

   for (i=0;i<SIZE;i++){
      j=0;
      do{
         c = getc(fp);
         if ((isdigit(c)) || (c== ' ')){
            board[i][j] = c; 
            j++;
         }    
      }while (j<SIZE);
      getc(fp);
   }
   fclose(fp);
   validboard(board);   
   findspace(board, x, y); 
}



void validboard(char board[SIZE][SIZE])
{
   int i, j, k = 0, cnt;
   char valid[] = {'1','2','3','4','5','6','7','8',' '};

   do{
      cnt = 0;
      for (i=0;i<SIZE;i++){
         for (j=0;j<SIZE;j++){
            if (board[i][j] == valid[k]){
               cnt++;
            }
         }
      }
      if (cnt != 1){
         fprintf(stderr, "Invalid board.\n"); /*If the cell does not equal one of the valid[] elements, the board is invalid*/
         exit(1);
      }
      k++;
   } while (((cnt==0) || (cnt==1)) && (k<CELLS));

}

/* Finds the coordinates of the space to begin swapping */
void findspace(char board[SIZE][SIZE], int *x, int *y)
{
   int j, i;

   for (i=0;i<SIZE;i++){ 
      for(j=0;j<SIZE;j++){
         if (board[i][j] == ' '){
            *x = i;
            *y = j;
         }
      }
   }
}

void copyboard(char board[SIZE][SIZE], char copy[SIZE][SIZE])
{
   int i, j;

   for (i=0;i<SIZE;i++){
      for (j=0;j<SIZE;j++){
         board[i][j] = copy[i][j];
      }
   }
}

Node *AllocateBoard(char board[SIZE][SIZE])
{
   Node *ptr;

   ptr = (Node *)malloc(sizeof(Node));

   if (ptr==NULL){
      fprintf(stderr, "Cannot allocate board.\n");
   }
   copyboard(ptr->board, board);
   ptr->next = NULL;
   ptr->parent = NULL; /*Automatically set parent to NULL for front board, this gets changed for later boards*/
   return ptr;

}

void iterate(int *x, int *y, Node *front)
{
   int cnt=0;
   Node *current = front;
   Node *back = front->next;

   while (back!=current){ /*If it has swapped all tiles and not produced the solution board, the board is deemed not solvable*/
      back = explore(current->board, x, y, front, current); 
      current = current->next; /*Moves pointer along by one each time producing child boards*/
      cnt++;
      findspace(current->board, x, y);
   }
   if (back==current){
      fprintf(stderr, "Board not solvable.\n");
      exit(1); 
   }
}

/* Swaps tiles right, left, down, up after checking bounds */
Node *explore(char board[SIZE][SIZE], int *x, int *y, Node *front, Node *current)
{
   Node *par = current; /*par points to parent node*/

   if ((*x+1) < SIZE){ /*Swap right*/
      swap(board[*x]+*y, (board[*x+1]+*y));/*Swap space and cell right if in bounds*/
      current = AllocateBoard(board); /*Allocate space to swapped board*/
      current->parent = par; /*Assign pointer to parent board so that we can backtrack from solution for printing*/
      swap(board[*x]+*y, (board[*x+1]+*y));/*Swap back to parent board for next swap*/
      storeboard(front, current);/* Store child board in queue*/
   }
   if ((*x-1) >= 0){ /*Swap left*/
      swap(board[*x]+*y, (board[*x-1]+*y));
      current = AllocateBoard(board);
      current->parent = par;
      swap(board[*x]+*y, (board[*x-1]+*y));
      storeboard(front, current);
   }
   if ((*y-1) >= 0){ /*Swap down*/
      swap((board[*x]+*y), board[*x]+(*y-1));
      current = AllocateBoard(board);
      current->parent = par;
      swap(board[*x]+*y, board[*x]+(*y-1));
      storeboard(front, current);
   }
   if ((*y+1) < SIZE){ /*Swap up*/
      swap((board[*x]+*y), (board[*x]+(*y+1)));
      current = AllocateBoard(board);
      current->parent = par;
      swap(board[*x]+*y, board[*x]+(*y+1));
      storeboard(front, current);   
   } 

   return current;

}

int storeboard(Node *front, Node *current)
{
   Node *prev = front;
   char original[SIZE][SIZE] = {{'1','2','3'},{'4','5','6'},{'7','8',' '}};
   
   if (compare(current->board, original)){/*Check if solution board has been achieved*/
      printsolution(current, front);
   }

   do{
      if (compare(current->board, prev->board)){
         free(current);/*Do not store board if it is already in queue*/
         return 0;
      }
      if (prev->next!=NULL){
        prev = prev->next;
      }
   }while (prev->next!=NULL);

   prev->next = current;/*Add board to queue*/
   return 1;
   
}

void printsolution(Node *current, Node *front)
{
   int mvs=0;

   mvs = printreverse(current, front);/*Print number of moves and the solution sequence*/
   freeboard(front);/*Free space when finished*/
   printf("\n\n");
   printf("Moves = %d\n", mvs-1); 
   Neill_NCURS_Delay(DELAY+DELAY);  /* Delay before ending to show number of moves */
   atexit(Neill_NCURS_Done);  /*Call this function if we exit() anywhere in the code */
   exit(1);
  
}


int printreverse(Node *current, Node *front)
{
   static int mvs=0;
   char screen[SCREEN_SIZE][SCREEN_SIZE] = {{' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                            {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                            {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                            {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                            {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                            {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                            {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                            {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                            {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                            {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                            {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                            {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                            {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                            {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                            {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '}};

   NCURS_Simplewin sw;

   /*Colour and style information*/
   Neill_NCURS_Init(&sw);
   Neill_NCURS_CharStyle(&sw, " ", COLOR_GREEN, COLOR_GREEN, A_BOLD | A_STANDOUT);
   Neill_NCURS_CharStyle(&sw, "1", COLOR_GREEN, COLOR_BLUE, A_BOLD | A_STANDOUT);
   Neill_NCURS_CharStyle(&sw, "2", COLOR_GREEN, COLOR_BLUE, A_BOLD | A_STANDOUT);
   Neill_NCURS_CharStyle(&sw, "3", COLOR_GREEN, COLOR_BLUE, A_BOLD | A_STANDOUT);
   Neill_NCURS_CharStyle(&sw, "4", COLOR_GREEN, COLOR_BLUE, A_BOLD | A_STANDOUT);
   Neill_NCURS_CharStyle(&sw, "5", COLOR_GREEN, COLOR_BLUE, A_BOLD | A_STANDOUT);
   Neill_NCURS_CharStyle(&sw, "6", COLOR_GREEN, COLOR_BLUE, A_BOLD | A_STANDOUT);
   Neill_NCURS_CharStyle(&sw, "7", COLOR_GREEN, COLOR_BLUE, A_BOLD | A_STANDOUT);
   Neill_NCURS_CharStyle(&sw, "8", COLOR_GREEN, COLOR_BLUE, A_BOLD | A_STANDOUT);
   
   if (current == NULL){
      return mvs;
   }   
   else{
      printreverse(current->parent, front);/*Recursive printing from start board to solution board*/
      makescreen(current->board, screen); /*Enlarge board for printing*/
      Neill_NCURS_PrintArray(&screen[0][0], SCREEN_SIZE, SCREEN_SIZE, &sw);
      Neill_NCURS_Delay(DELAY);  /* Delay between each iteration*/
      Neill_NCURS_Events(&sw);  /* Wait for mouse click, or ESC key */
      if (sw.finished){
         freeboard(front);
         exit(1);
      }
      mvs++;/*Count moves from start board to solution board*/
   }
   return mvs;
}

/*Copies board numbers into a bigger array "screen"*/
void makescreen(char board[SIZE][SIZE], char screen[SCREEN_SIZE][SCREEN_SIZE])
{
   int a=START, b=START;
   int k, j;

   for (k=0;k<SIZE;k++){
      b=START;
      for(j=0;j<SIZE;j++){
         screen[a][b] = board[k][j];
         b+=SHIFT;
      }
      a+=SHIFT;
   }

}

/*Free space at the end of program*/
void freeboard(Node *front)
{
   Node *temp;

   while (front!=NULL){
      temp = front;
      front = front->next;
      free(temp);
   }

}

int compare(char board[SIZE][SIZE], char original[SIZE][SIZE])
{
   int i, j, cnt = 0;

   for (i=0;i<SIZE;i++){
      for(j=0;j<SIZE;j++){
         if (board[i][j] == original[i][j]){
            cnt++;
         }
      }
   }
   if (cnt == CELLS){ /*If all cells are the same, boards are identical*/
      return 1;
   }
   else {
      return 0;
   }   
}

void swap(char *a, char *b)
{
   char tmp;

   tmp = *a;
   *a = *b;
   *b = tmp;
}

void printboard(char board[SIZE][SIZE])
{
   int i, j;

   for (i=0;i<SIZE;i++){
      for (j=0;j<SIZE;j++){
         printf("%c", board[i][j]);
      }
      printf("\n");
   }
}

