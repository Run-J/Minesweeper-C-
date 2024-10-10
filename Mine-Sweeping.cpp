// author: Run
// first version: 12/19/2023
// description: A console minesweeper mini-game written in C code. 
//				The user can choose from three levels of difficulty, 
//				each corresponding to a different number of mines. 
//				In addition to provide marking function.
//				****Note: Use 666-666 to cheat and see the number of nearby mines.****

#pragma warning (disable:4996)

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define ROW 9
#define COL 9

#define ROWS ROW + 2
#define COLS COL + 2

#define MODE 10

void menu();
void game(int level);
void initBoard(char board[ROWS][COLS], int rows, int cols, char form);
void displayBoard(char board[ROWS][COLS], int row, int col);
void setMine(char board[ROWS][COLS], int row, int col, int mineMode);
void findMine(char mine[ROWS][COLS], char show[ROWS][COLS], int row, int col, int mineMode);
int getMineCount(char board[ROWS][COLS], int x, int y);
int expandZeroes(char mine[ROWS][COLS], char show[ROWS][COLS], int x, int y);
void makeMark(char show[ROWS][COLS], int x, int y, int* remainingMines);


int main(void)
{
	char buffer[10] = "";
	int input = 0;
	int level = 0;
	srand((unsigned int)time(NULL));

	do
	{
		menu();
		printf("Start? Choose 1 or 0: \n");
		fgets(buffer, sizeof(buffer), stdin);
		sscanf(buffer, "%d", &input);

		switch (input)
		{
		case 1:
			printf("The game start! Please chose your level: 1-3\n");
			fgets(buffer, sizeof(buffer), stdin);
			sscanf(buffer, "%d", &level);
			game(level);
			break;
		case 0:
			printf("Take a break\n");
			break;
		default:
			fprintf(stderr, "Please choose 1 or 0\n");
			break;
		}

	} while (input);

	return 0;
}


void menu()
{
	printf("****************\n");
	printf("*** 1. Play  ***\n");
	printf("*** 0. Exit  ***\n");
	printf("****************\n");
}


void game(int level)
{
	int mineMode = 0;
	int boardMode = 0;
	switch (level)
	{
	case 1:
		mineMode = 0 + MODE;
		boardMode = 0 + ROWS; 
		break;
	case 2:
		mineMode = 30 + MODE;
		boardMode = 7 + ROWS;
		break;
	case 3:
		mineMode = 89 + MODE;
		boardMode = 16 + ROWS;
		break;
	}

	//1. Following inital game board
	char mine[ROWS][COLS] = { 0 }; // Information on deposited mines
	char show[ROWS][COLS] = { 0 }; // Storage of cleared mines
	// Mine arrays are '0' when no mines are laid.
	initBoard(mine, ROWS, COLS, '0');
	// Show array is always '*' when no mines are laid
	initBoard(show, ROWS, COLS, '*');


	//2. Following set mine
	setMine(mine, ROW, COL, mineMode);
	//displayBoard(mine, ROW, COL); // Minefields should be hidden. Remove the comment markers if player want cheat
	displayBoard(show, ROW, COL);


	//3. Following check mines and show how many mines are around
	findMine(mine, show, ROW, COL, mineMode);
}


void initBoard(char board[ROWS][COLS], int rows, int cols, char form)
{
	int i = 0;
	int j = 0;
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			board[i][j] = form;
		}
	}
}


void displayBoard(char board[ROWS][COLS], int row, int col)
{
	int i = 0;
	int j = 0;
	printf("-------------------\n");
	for (i = 1; i <= row; i++)
	{
		printf("%d ", ROWS - 1 - i); // print number of rows (y-axis)
		for (j = 1; j <= col; j++)
		{
			printf("%c ", board[i][j]);
		}
		printf("\n");
	}
	printf("%2s1 2 3 4 5 6 7 8 9\n", ""); // print number of columns (x-axis)
	printf("-------------------\n");

}


void setMine(char board[ROWS][COLS], int row, int col, int mineMode)
{
	int count = mineMode;
	while (count)
	{
		int x = rand() % row + 1;
		int y = rand() % col + 1;

		if (board[x][y] == '0')
		{
			board[x][y] = '1';
			count--;
		}
	}
}


int getMineCount(char board[ROWS][COLS], int x, int y)
{
	int count = 0;

	for (int i = x - 1; i <= x + 1; i++)
	{
		for (int j = y - 1; j <= y + 1; j++)
		{
			if (i >= 0 && i <= ROWS - 1 && j >= 0 && j <= COLS - 1 && !(i == x && j == y))
			{
				if (board[i][j] == '1')
				{
					count++;
				}
			}
		}
	}

	return count;

	//Folowing is second method
	// 
	//int get_mine_count(char board[ROWS][COLS], int x, int y)
	//{
	//	return board[x - 1][y] +
	//		board[x - 1][y - 1] +
	//		board[x][y - 1] +
	//		board[x + 1][y - 1] +
	//		board[x + 1][y] +
	//		board[x + 1][y + 1] +
	//		board[x][y + 1] +
	//		board[x - 1][y + 1] - 8 * '0';
	//}

}


int expandZeroes(char mine[ROWS][COLS], char show[ROWS][COLS], int x, int y)
{
	if (x <= 0 || x >= ROWS - 1 || y <= 0 || y >= COLS - 1 || show[x][y] != '*')
	{
		return 0;
	}

	int revealed = 1;
	int count = getMineCount(mine, x, y); //check how many mines around
	show[x][y] = (count > 0) ? count + '0' : ' ';

	if (count == 0)
	{
		for (int i = x - 1; i <= x + 1; i++)
		{
			for (int j = y - 1; j <= y + 1; j++)
			{
				revealed = revealed + expandZeroes(mine, show, i, j);
			}
		}
	}

	return revealed;
}


void findMine(char mine[ROWS][COLS], char show[ROWS][COLS], int row, int col, int mineMode)
{
	int x = 0;
	int y = 0;
	int exchange = 0;
	int remaining = row * col - mineMode;
	int count = 0; // a number of mines
	char buffer[10] = "";
	int remainingMines = mineMode;
	
	while (remaining > 0)
	{
		printf("Remaining mines: %d\n", remainingMines);
		printf("Please enter the coordinates you want to check or mark (format: 'x'-'y' or 'mark x-y'): \n");
		fgets(buffer, sizeof(buffer), stdin);
		if (strncmp(buffer, "mark ", 5) == 0) // mark 
		{	
			sscanf(buffer + 5, "%d-%d", &x, &y);
			exchange = y;
			y = x;
			x = ROWS - 1 - exchange;
			makeMark(show, x, y, &remainingMines);
			continue;
		}
		else
		{
			if (sscanf(buffer, "%d-%d", &x, &y) == 2) // without mark
			{
				if (x == 666, y == 666)
				{
					displayBoard(mine, ROW, COL);
					continue;
				}
				exchange = y;
				y = x;
				x = ROWS - 1 - exchange;
			}
			else
			{
				fprintf(stderr, "Nothing have changed, please enter correct format\n");
				continue;
			}
		}
		

		if (x >= 1 && x <= row && y >= 1 && y <= col)
		{
			if (show[x][y] != '*')
			{
				printf("This coordinate you have just checked, please select another coordinate.\n");
			}
			else
			{
				if (mine[x][y] == '1')
				{
					printf("Boom! Oops! Unfortunatly, you stepped on a mine. Let's see the answer\n\n");
					displayBoard(mine, ROW, COL);
					printf("\n");
					break;
				}
				else
				{
					remaining = remaining - expandZeroes(mine, show, x, y);
					displayBoard(show, ROW, COL);
				}
			}
		}
		else
		{
			fprintf(stderr, "Please enter valid range from 1 to %d\n", ROW);
		}
	}

	if (remaining == 0)
	{
		printf("Congratulations! You won!\n\n");
		displayBoard(mine, ROW, COL);
	}
}


void makeMark(char show[ROWS][COLS], int x, int y, int* remainingMines)
{
	if (x <= ROW && x >= 1 && y <= COL && y >= 1)
	{
		if (show[x][y] == '*')
		{
			show[x][y] = 'M'; // make a mark
			(*remainingMines)--;

		}
		else if (show[x][y] == 'M')
		{
			show[x][y] = '*'; // remove the mark
			(*remainingMines)++;
		}
	}
	displayBoard(show, ROW, COL);
}
