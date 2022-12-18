#include <stdio.h>
#include <stdbool.h>
#ifndef N
#define N 8
#endif

bool board[N][N];
FILE *fp;
unsigned long long int total = 0;

void clearBoard()
{
	for (int y = 0; y < N; y++)
	{
		for (int x = 0; x < N; x++)
			board[y][x] = false;
	}
}

bool check(int x, int y)
{
	for (int i = 0; i < N; i++)
	{
		if(x - i >= 0)
		{
			if(board[y][x - i]) return false;
			if(y - i >= 0)
				if(board[y - i][x - i]) return false;
			if(y + i < N)
				if(board[y + i][x - i]) return false;
		}	
	}
	return true;
}

void printBoard()
{
	for (int y = 0; y < N; y++)
	{
		for (int x = 0; x < N; x++)
			fprintf(fp, "%d", board[y][x]);
		fprintf(fp, "\n");
	}
	fprintf(fp, "\n");
}

void solve(int col)
{
	if (col == N)
	{
		printBoard();
		total ++;
		return;
	}
	for (int y = 0; y < N; y++)
	{
		if (check(col, y))
		{
			board[y][col] = true;
			solve(col + 1);
			board[y][col] = false;
		}
	}
}

int main()
{
	fp = fopen("all_solutions.txt", "w");
	clearBoard();
	solve(0);
	fprintf(fp, "----------------\nTotal: %lld\n", total);
	fclose(fp);
	printf("Total: %lld\n", total);
	return (0);
}
