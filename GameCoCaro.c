#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_SIZE 15

const char HORZ = 196;
const char VERT = 179;
const char TL = 218;
const char TM = 194;
const char TR = 191;
const char BL = 192;
const char BM = 193;
const char BR = 217;
const int numW = 3;  // Width for writing out an integer
const int charW = 1; // Width for writing out a character
const int numPerLine = 15;
const char VERT_START = 195;
const char VERT_END = 180;
const char VERT_MID = 197;
const char *inputCommand = "Previous move/Next move/Stop [p/n/s]: ";
const char *endOfHistory = "This is the end of the match.\nPrevious move/Next move/Stop [p/n/s]: ";
const char *startOfGame = "This is the start of the match.\nPrevious move/Next move/Stop [p/n/s]: ";
const char *invalidInput = "Illegal input, please try again: ";

enum Stone
{
    NA,
    X,
    O
};

struct Move
{
    int row;
    int col;
    enum Stone stone;
};

void startGame();
void displayHistory(char *history, int numOfMoves);

static char *lineString(char left, char mid, char right, char horz, int wcell, int ncell);
void displayBoard(enum Stone arr[][15], int size);
void displayBoardSimple(enum Stone arr[][15], int size);

bool makeMove(enum Stone board[][MAX_SIZE], int size, char *playerMove, bool isFirstPlayerTurn);
bool hasWon(enum Stone board[][15], int size, bool isFirstPlayerTurn);

int main()
{

    printf("Welcome to Gomoku!\n");
    printf("1. Play game\n");
    printf("2. History\n");
    printf("3. Exit\n");
    printf("Please select mode [1/2/3]: ");
    int option = 0;
    do
    {
        fflush(stdin);
        scanf("%d", &option);
        if (option == 1)
        {
            startGame();
        }
        else if (option == 2)
        {
            int numOfMoves;
            printf("Please enter number of moves: ");
            scanf("%d", &numOfMoves);
            char history[700];
            printf("Please enter history: ");
            scanf("%s", history);
            displayHistory(history, numOfMoves);
        }
        else if (option == 3)
        {
            return 0;
        }
        else
        {
            printf("Invalid mode, please try again: ");
        }
    } while (option != 1 && option != 2 && option != 3);

    return 0;
}

void startGame()
{
    enum Stone game[MAX_SIZE][MAX_SIZE];
    for (int i = 0; i < MAX_SIZE; i++)
    {
        for (int j = 0; j < MAX_SIZE; j++)
        {
            game[i][j] = NA;
        }
    }
    displayBoard(game, MAX_SIZE);
    char playerMove[50];
    bool player1Turn = true;
    printf("Player 1 turn: ");
    while (true)
    {
        scanf("%s", playerMove);
        if (strcmp(playerMove, "ff") == 0)
        {
            printf("%s", ((player1Turn) ? "Player 2 won " : "Player 1 won "));
            break;
        }
        else if (makeMove(game, MAX_SIZE, playerMove, player1Turn))
        {
            if (hasWon(game, MAX_SIZE, player1Turn))
            {
                displayBoard(game, MAX_SIZE);
                printf("%s", ((player1Turn) ? "Player 1 won " : "Player 2 won "));
                return;
            }
            else
            {
                player1Turn = !player1Turn;
                displayBoard(game, MAX_SIZE);
                printf("%s", ((player1Turn) ? "Player 1 turn: " : "Player 2 turn: "));
            }
        }
        else
        {
            printf("Illegal move, please try again: ");
        }
    }
}

static char *lineString(char left, char mid, char right, char horz, int wcell, int ncell)
{
    static char result[100];
    strcpy(result, "");
    char cell[10];
    for (int i = 0; i < wcell; i++)
    {
        cell[i] = horz;
    }
    cell[wcell] = '\0';
    strncat(result, &left, 1);
    char cellMid[20];
    strcpy(cellMid, cell);
    strncat(cellMid, &mid, 1);
    for (int i = 0; i < ncell - 1; i++)
        strcat(result, cellMid);
    char cellRight[20];
    strcpy(cellRight, cell);
    strncat(cellRight, &right, 1);
    strcat(result, cellRight);
    return result;
}

void displayBoard(enum Stone arr[][15], int size)
{
    // UPPERLINE
    printf("   ");
    for (int i = 0; i < size; i++)
    {
        printf("  %c ", (char)(i + 'a'));
    }
    printf("\n");
    printf("   %s\n", lineString(TL, TM, TR, HORZ, 3, numPerLine));

    // MIDDLE
    for (int i = 0; i < size; i++)
    {
        printf("%2d %c", size - i, VERT);
        for (int j = 0; j < size; j++)
        {
            if (arr[i][j] == NA)
                printf("   %c", VERT);
            if (arr[i][j] == X)
                printf(" X %c", VERT);
            if (arr[i][j] == O)
                printf(" O %c", VERT);
        }
        printf("\n");
        if (i != size - 1)
            printf("   %s\n", lineString(VERT_START, VERT_MID, VERT_END, HORZ, 3, numPerLine));
    }

    // UNDERLINE
    printf("   %s\n", lineString(BL, BM, BR, HORZ, 3, numPerLine));
}

void displayBoardSimple(enum Stone arr[][15], int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            printf("%d", arr[i][j]);
        }
    }
    printf("\n");
}
bool makeMove(enum Stone board[][MAX_SIZE], int size, char *playerMove, bool isFirstPlayerTurn)
{
    int row, col;
    if (strlen(playerMove) == 2 && playerMove[0] >= '0' && playerMove[0] <= '9' && playerMove[1] >= 'a' && playerMove[1] <= ('a' + MAX_SIZE - 1)) // playerMove[1] >= '0' && playerMove[1] <= '9'
    {
        row = MAX_SIZE - (int)(playerMove[0] - '0');
        col = (int)(playerMove[1] - 'a');
    }
    else if (strlen(playerMove) == 3 && playerMove[0] >= '0' && playerMove[0] <= '9' && playerMove[1] >= '0' && playerMove[1] <= '9' && playerMove[2] >= 'a' && playerMove[2] <= ('a' + MAX_SIZE - 1))
    {
        row = MAX_SIZE - ((int)(playerMove[0] - '0') * 10 + (int)(playerMove[1] - '0'));
        col = (int)(playerMove[2] - 'a');
    }
    else
    {
        return false;
    }
    if (board[row][col] == NA)
    {
        if (isFirstPlayerTurn == true) // player number 1
        {

            board[row][col] = X;
        }
        else // player number 2
        {
            board[row][col] = O;
        }
        return true;
    }
    else
    {
        return false;
    }
    return true;
}

bool hasWon(enum Stone board[][MAX_SIZE], int size, bool isFirstPlayerTurn)
{
    bool won = false;
    int count = 1;
    int h, k;
    // check won in row
    for (int i = 0; i < MAX_SIZE; i++)
    {
        count = 1;
        for (int j = 0; j < MAX_SIZE; j++)
        {
            if (board[i][j] == board[i][j + 1] && board[i][j] != NA)
            {
                count++;
                h = i;
                for (k = j + 1; k < MAX_SIZE; k++)
                {
                    if (board[h][k] == board[h][k + 1] && board[h][k] != NA)
                    {
                        count++;
                        if (count == 5)
                        {
                            if (!(board[i][j] != board[i][j - 1] && board[i][j - 1] != NA && board[h][k + 1] != board[h][k + 2] && board[h][k + 2] != NA))
                            {
                                if (!(board[h][k + 1] == board[h][k + 2] && board[h][k + 2] != NA))
                                {
                                    if (!(board[i][j] == board[i][j - 1] && board[i][j - 1] != NA))
                                    {
                                        won = true;
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }
            else
            {
                count = 1;
            }
        }
    }

    // check won in column
    for (int j = 0; j < MAX_SIZE; j++)
    {
        count = 1;
        for (int i = 0; i < MAX_SIZE; i++)
        {
            if (board[i][j] == board[i + 1][j] && board[i][j] != NA)
            {
                count++;
                k = j;
                for (h = i + 1; h < MAX_SIZE; h++)
                {
                    if (board[h][k] == board[h + 1][k] && board[h][k] != NA)
                    {
                        count++;
                        if (count == 5)
                        {
                            if (!(board[i][j] != board[i - 1][j] && board[i - 1][j] != NA && board[h + 1][k] != board[h + 2][k] && board[h + 2][k] != NA))
                            {
                                if (!(board[h + 1][k] == board[h + 2][k] && board[h + 2][k] != NA))
                                {
                                    if (!(board[i][j] == board[i - 1][j] && board[i - 1][j] != NA))
                                    {
                                        won = true;
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }
            else
            {
                count = 1;
            }
        }
    }
    // check won in diagonal left to right

    for (int i = 0; i < MAX_SIZE; i++)
    {
        count = 1;
        for (int j = 0; j < MAX_SIZE; j++)
        {
            if (board[i][j] == board[i + 1][j + 1] && board[i][j] != NA)
            {
                count++;
                h = i;
                for (k = j + 1; k < MAX_SIZE; k++)
                {
                    h++;
                    if (h < MAX_SIZE)
                    {

                        if (board[h][k] == board[h + 1][k + 1] && board[h][k] != NA)
                        {
                            count++;
                            if (count == 5)
                            {
                                if (!(board[i][j] != board[i - 1][j - 1] && board[i - 1][j - 1] != NA && board[h + 1][k + 1] != board[h + 2][k + 2] && board[h + 2][k + 2] != NA))
                                {
                                    if (!(board[h + 1][k + 1] == board[h + 2][k + 2] && board[h + 2][k + 2] != NA))
                                    {
                                        if (!(board[i][j] == board[i - 1][j - 1] && board[i - 1][j - 1] != NA))
                                        {
                                            won = true;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }
            else
            {
                count = 1;
            }
        }
    }
    // check won in diagonal right to left
    for (int i = 0; i < MAX_SIZE; i++)
    {
        count = 1;
        for (int j = 0; j < MAX_SIZE; j++)
        {
            if (board[i][j] == board[i + 1][j - 1] && board[i][j] != NA)
            {
                count++;
                h = i;
                for (k = j - 1; k >= 0; k--)
                {
                    h++;
                    if (h >= 0)
                    {

                        if (board[h][k] == board[h + 1][k - 1] && board[h][k] != NA)
                        {
                            count++;
                            if (count == 5)
                            {
                                if (!(board[i][j] != board[i - 1][j + 1] && board[i - 1][j + 1] != NA && board[h + 1][k - 1] != board[h + 2][k - 2] && board[h + 2][k - 2] != NA))
                                {
                                    if (!(board[h + 1][k - 1] == board[h + 2][k - 2] && board[h + 2][k - 2] != NA))
                                    {
                                        if (!(board[i][j] == board[i - 1][j + 1] && board[i - 1][j + 1] != NA))
                                        {
                                            won = true;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }
            else
            {
                count = 1;
            }
        }
    }

    return won;
}

void displayHistory(char *history, int numOfMoves)
{
    enum Stone game[MAX_SIZE][MAX_SIZE];
    for (int i = 0; i < MAX_SIZE; i++)
    {
        for (int j = 0; j < MAX_SIZE; j++)
        {
            game[i][j] = NA;
        }
    }
    displayBoardSimple(game, MAX_SIZE);

    int T[2][255];
    int t = 0;
    for (int i = 0; i < numOfMoves; i++)
    {
        if (history[t + 1] <= '9' && history[t + 1] >= '0')
        {
            T[0][i] = 15 - ((int)(history[t] - '0') * 10 + (int)(history[t + 1] - '0'));
            T[1][i] = (int)(history[t + 2] - 'a');
            t += 3;
        }
        else
        {
            T[0][i] = 15 - (int)(history[t] - '0');
            T[1][i] = (int)(history[t + 1] - 'a');
            t += 2;
        }
    }
    printf("%s", inputCommand);
    char input[2];
    t = 0;
    while (true)
    {
        scanf("%s", &input);
        if (input[0] == 'n')
        {
            if (t == numOfMoves)
            {
                printf("%s", endOfHistory);
            }
            else
            {
                enum Stone next;
                if (t % 2 == 0)
                {
                    next = X;
                }
                else
                {
                    next = O;
                }
                game[(T[0][t])][(T[1][t])] = next;
                displayBoardSimple(game, MAX_SIZE);
                t++;
                printf("%s", inputCommand);
            }
        }
        else if (input[0] == 'p')
        {
            if (t == 0)
            {
                printf("%s", startOfGame);
            }
            else
            {
                t--;
                game[(T[0][t])][(T[1][t])] = NA;
                displayBoardSimple(game, MAX_SIZE);
                printf("%s", inputCommand);
            }
        }
        else if (input[0] == 's')
        {
            return;
        }
        else
        {
            printf("%s", invalidInput);
        }
    }
}