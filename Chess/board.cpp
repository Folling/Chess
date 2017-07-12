#include "board.h"

const int startup[ranks][files] ={  { R,  N,  B,  Q,  K,  B,  N,  R, },
							{ P,  P,  P,  P,  P,  P,  P,  P, },
							{ 0,  0,  0,  0,  0,  0,  0,  0, },
							{ 0,  0,  0,  0,  0,  0,  0,  0, },
							{ 0,  0,  0,  0,  0,  0,  0,  0, },
							{ 0,  0,  0,  0,  0,  0,  0,  0, },
							{-P, -P, -P, -P, -P, -P, -P, -P, },
							{-R, -N, -B, -Q, -K, -B, -N, -R  }
						 };

position board;

void setup()
{
	for(int i = 0; i < ranks; i++)
	{
		for(int j = 0; j < files; j++)
		{
			board[i][j] = startup[i][j];
		}
	}
}

char associatedChar(int i)
{
	char retChar;
	switch (abs(i)) {
	case 1000 : retChar = 'P';
		break;
	case 3000 : retChar = 'N';
		break;
	case 3001 : retChar = 'B';
		break;
	case 5000 : retChar = 'R';
		break;
	case 9000 : retChar = 'Q';
		break;
	case 25000: retChar = 'K';
		break;
	default   : retChar = '0';
	}
	return i < 0 ? tolower(retChar) : retChar;
}

void printBoard()
{
	for(int i = ranks-1; i >= 0; i--)
	{
		for (int j = 0; j < files; j++) {
			std::cout << associatedChar(board[i][j]);
		}
		std::cout << std::endl;
	}
}