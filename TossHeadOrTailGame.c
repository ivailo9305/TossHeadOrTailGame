#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>

int pdes1[2];
int pdes2[2];
int pdes3[2];

int player1Score;
int player2Score;
int player3Score;

int player1Active = 1;
int player2Active = 1;
int player3Active = 1;

int gameOver = 0;
int winner = 0;
int gameCount = 0;

int checkResults(int a, int b, int c)
{
	if (a > b && a > c)
	{
		winner = 1;
		return 1;
	}
	else if (b > a && b > c)
	{
		winner = 2;
		return 1;
	} 
	else if (c > a && c > b)
	{
		winner = 3;
		return 1;
	}
	else if (a == b && a == c)
	{
		return 0;
	}
	else if (a == b && a > c)
	{
		player3Active = 0;
		return 0;
	}
	else if (a == c && a > b)
	{
		player2Active = 0;
		return 0;
	}
	else if (c == b && c > a)
	{
		player1Active = 0;
		return 0;
	}
}

int tossCoins()
{
	int heads = 0;
	int toss;
	while (1)
	{
		toss = rand() % 100;
		if (toss > 50)
		{
			heads++;
		}
		else
		{
			break;
		}
		return heads;
	}
}

void play()
{
	do
	{
		if (player1Active != 0)
		{
			pipe(pdes1);
			if (fork() == 0)
			{ /* child */
				srand(getpid());
				close(pdes1[0]);
				int result = tossCoins();
				write(pdes1[1], &result, sizeof(result));
				close(pdes1[1]);	
				exit(EXIT_SUCCESS);
			}
			else
			{
				wait(NULL);
				close(pdes1[1]);
				read(pdes1[0], &player1Score, sizeof(player1Score));
				//printf("Player 1 score: %d\n", player1Score);
				close(pdes1[0]);
			}
		}

		if (player2Active != 0)
		{
			pipe(pdes2);
			if (fork() == 0)
			{ /* child */
				srand(getpid());
				close(pdes2[0]);
				int result = tossCoins();
				write(pdes2[1], &result, sizeof(result));
				close(pdes2[1]);
				exit(EXIT_SUCCESS);
			}
			else
			{
				wait(NULL);
				close(pdes2[1]);
				read(pdes2[0], &player2Score, sizeof(player2Score));
				//printf("Player 2 score: %d\n", player2Score);
				close(pdes2[0]);
			}
		}

		if (player3Active != 0)
		{
			pipe(pdes3);
			if (fork() == 0)
			{ /* child */
				srand(getpid());
				close(pdes3[0]);
				int result = tossCoins();
				write(pdes3[1], &result, sizeof(result));
				close(pdes3[1]);
				exit(EXIT_SUCCESS);
			}
			else
			{
				wait(NULL);
				close(pdes3[1]);
				read(pdes3[0], &player3Score, sizeof(player3Score));
				//printf("Player 3 score: %d\n", player3Score);
				close(pdes3[0]);
			}
		}
		gameCount++;
		gameOver = checkResults(player1Score, player2Score, player3Score);
	} while (gameOver == 0);
	
	printf("The winner after %d games is Player %d\n", gameCount, winner);
	printf("Final results: \n");
	printf("Player 1 Score : %d\n", player1Score);
	printf("Player 2 Score : %d\n", player2Score);
	printf("Player 3 Score : %d\n", player3Score);
}
int main(void)
{
	play();
}
