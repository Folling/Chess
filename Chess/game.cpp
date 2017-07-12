#include "game.h"

void runGame()
{
	std::string move;
	player currPlayer = WHITE;
	while (true)
	{
		std::cout << "Current player: " << ((currPlayer == WHITE) ? "white" : "black") << std::endl;
		std::cin >> move;
		if (move == "reset") {
			setup();
			printBoard();
			continue;
		}
		if (move == "skip") {
			currPlayer *= -1;
			printBoard();
			continue;
		}
		while (true)
		{
			for (unsigned int i = 0; i < move.size(); i++)
			{
				move.at(i) = tolower(move.at(i));
			}
			if (
				move.size() != 4
				|| move[0] < 'a' || move[0] > 'h'
				|| static_cast<int>(move[1]) < 49 || static_cast<int>(move[1]) > 56
				|| move[2] < 'a' || move[2] > 'h'
				|| static_cast<int>(move[3]) < 49 || static_cast<int>(move[3]) > 56
				)
			{
				std::cout << "Unable to parse move - " << move << std::endl;
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cin >> move;
			}
			else break;
		}
		int fromRank = static_cast<int>(move[1]) - 49;
		int fromFile = abs('a' - move[0]);
		int toRank = static_cast<int>(move[3]) - 49;
		int toFile = abs('a' - move[2]);
		if (!canMove(fromRank, fromFile, toRank, toFile, false, currPlayer))
		{
			std::cout << "Invald move, try again! Move interpreted: " << fromFile << fromRank << toFile << toRank << std::endl;
		}
		else
		{
			board[toRank][toFile] = board[fromRank][fromFile];
			board[fromRank][fromFile] = 0;
			std::cout << std::endl;
			currPlayer *= -1;
			printBoard();
		}
		int KingRank = (currPlayer == WHITE) ? WhiteKing.first  : BlackKing.first;
		int KingFile = (currPlayer == WHITE) ? WhiteKing.second : BlackKing.second;
		switch (checkState(KingRank, KingFile, currPlayer))
		{
		case NO_STATE:
			break;
		case CHECK: std::cout << "Check!\n";
			break;
		case MATE: std::cout << "Checkmate!\n";
			break;
		case STALEMATE: std::cout << "Stalemate!\n";
			break;
		default:
			break;
		}
	}
}