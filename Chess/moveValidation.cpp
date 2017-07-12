#include "moveValidation.h"


int abs(int i)
{
	return (i < 0) ? -i : i;
}

int sgn(int i)
{
	// signs are used to check for the same pieces
	// an empty square is not one of your pieces so 0 should return something unique
	if (i == 0) return 0;
	return (i < 0) ? -1 : 1;
}


bool canMove(int fromRank, int fromFile, int toRank, int toFile, bool checkForCheck, player p)
{
	// macro for the position
	int piece = board[fromRank][fromFile];
	if (piece == 0) return false;

	square king = p == WHITE ? WhiteKing : BlackKing;

	// you can't select your opponents pieces
#ifdef DEBUGMODE
	if (sgn(piece) != p) return false;
#endif

	// prohibits out of range errors
	if (fromRank < 0 || fromFile < 0 || toRank > 8 || toFile > 8) return false;

	// prohibits capturing your own pieces
	if (sgn(board[fromRank][fromFile]) == sgn(board[toRank][toFile])) return false;

	if(isCheck(king.first, king.second, p).first != NO_STATE)
	{
		int oldpiece = board[toRank][toFile];
		board[toRank][toFile] = piece;
		board[fromRank][fromFile] = 0;
		if (isCheck(king.first, king.second, p).first != NO_STATE)
		{
			board[toRank][toFile] = oldpiece;
			board[fromRank][fromFile] = piece;
			return false;
		}
	}

	// each piece has a specific movement this switches through all cases of piece
	switch (abs(piece)) // we'll use the absolute value as all pieces but pawns are able to move backwards
	{
	case PAWN:
		return checkPawn(fromRank, fromFile, toRank, toFile, checkForCheck);
	case KNIGHT:
		return checkKnight(fromRank, fromFile, toRank, toFile);
	case BISHOP:
		return checkBishop(fromRank, fromFile, toRank, toFile);
	case ROOK:
		return checkRook(fromRank, fromFile, toRank, toFile);
	case QUEEN:
		return checkQueen(fromRank, fromFile, toRank, toFile);
	case KING:	
		return checkKing(fromRank, fromFile, toRank, toFile, p);
	default: std::cout << "Selected Piece doesn't exist";
		return false;
	}
}

bool pieceInbetween(int fromRank, int fromFile, int toRank, int toFile, int piece)
{
	// determines which direction you're moving to
	int vertDir = (toRank > fromRank)? 1 : -1;
	int horDir  = (toFile > fromFile)? 1 : -1;
	switch (piece)
	{
	case BISHOP:
		// this loops once through the direction the bishop comes from all the way to one square before the
		// one you want to move to
		for (int i = vertDir, j = horDir; abs(i) != abs(toRank-fromRank); i += vertDir, j += horDir)
		{
				if (board[fromRank+i][fromFile+j] != 0) return true;
		}
		break;
	case ROOK:
		// this loops once through the direction (after checking whether you're moving vertically or horizontally
		// through all the squares until one square before the one you want to move to
		if (fromRank != toRank)
		{
			for (int i = vertDir; abs(i) != abs(toRank - fromRank); i += vertDir)
			{
				if (board[fromRank+i][fromFile] != 0) return true;
			}
		}
		else
		{
			for (int i = horDir; abs(i) != abs(toFile-fromFile); i += horDir)
			{
				if (board[fromRank][fromFile+i] != 0) return true;
			}
		}
	default:
		break;
	}
	return false;
}


bool checkPawn(int fromRank, int fromFile, int toRank, int toFile, bool checkForCheck)
{
	int piece = board[fromRank][fromFile];
	// pawns can't move backwards
	if (toRank < fromRank && sgn(piece) != -1) return false;
	if (toRank > fromRank && sgn(piece) !=  1) return false;

	// checks for whether you're trying to capture a piece
	// since pawns capture diagonally
	// this simply checks whether you're trying to capture diagonally
	if (abs(toRank - fromRank) == 1 && abs(toFile - fromFile) == 1) {
		// and then you return whether that square is empty or not
		// we don't need to check for same pieces as we already do that before
		return (board[toRank][toFile] != 0) ? true : false;
	}
	if (checkForCheck) return false;
	if (toFile != fromFile) return false;

	// a pawn can move two squares down/up if he is on his starting position, otherwise he can't.
	int verticalRange = (fromRank == 1 || fromRank == 6)? 2 : 1;
	if (abs(toRank - fromRank) > verticalRange) return false; // you can't move further than that

	//ToDo en-passent + Queening
	return true;
}

bool checkKnight(int fromRank, int fromFile, int toRank, int toFile)
{
	// a knight can only move 2 in one direction and then 1 in another 
	// this makes sure he doesn't move more than 2 in any direction
	if (abs(toRank - fromRank) > 2 || abs(toFile - fromFile) > 2) return false;

	// since he can only go 2 in one direction and 1 in another the total squares moved through would be 3	
	int totalMovement = abs(toRank - fromRank) + abs(toFile - fromFile);
	if (totalMovement != 3) return false;
	return true;
}

bool checkBishop(int fromRank, int fromFile, int toRank, int toFile)
{
	// checks whether the square you want to go to is on a diagonal with your current position
	if (abs(toRank - fromRank) != abs(toFile - fromFile)) return false;
	// then you check whether there is a piece inbetween you and your destination
	if (pieceInbetween(fromRank, fromFile, toRank, toFile, BISHOP) == true) return false;
	return true;
}

bool checkRook(int fromRank, int fromFile, int toRank, int toFile)
{
	// checks for vertical/horizontal movement
	if (fromRank != toRank && fromFile != toFile) return false;
	if (fromRank == toRank && fromFile == toFile) return false;
	// then you check whether there is a piece inbetween you and your destination
	if (pieceInbetween(fromRank, fromFile, toRank, toFile, ROOK) == true) return false;
	return true;
}

bool checkQueen(int fromRank, int fromFile, int toRank, int toFile)
{
	// basic determination whether this is part of the queen's moveset
	if (fromRank != toRank && fromFile != toFile
		&& abs(fromRank - toRank) != abs(fromFile - toFile)) return false;

	//checks for the direction and then determines whether a bishop/rook could move that way
	if (fromRank == toRank || fromFile == toFile)
	{
		return !pieceInbetween(fromRank, fromFile, toRank, toFile, ROOK);
	}
	if (fromRank != toRank && fromFile != toFile)
	{
		return !pieceInbetween(fromRank, fromFile, toRank, toFile, BISHOP);
	}
	return false;
}

bool checkKing(int fromRank, int fromFile, int toRank, int toFile, player p)
{
	// simply checks whether you're only moving one square in any direction 
	if (abs(fromRank - toRank) != 1 || abs(fromFile - toFile) != 1) return false;
	if (isCheck(toRank, toFile, p).first != NO_STATE) return false;

	// relocates the current King's position for further evaluation
	if(p == 1) WhiteKing = std::pair<int,int>(toRank, toFile);
	else BlackKing = std::pair<int, int>(toRank, toFile);
	
	return true;
}


state checkState(int KingRank, int KingFile, player p)
{
	//just the general information needed to process things
	checkInfo info = isCheck(KingRank, KingFile, p);

	int state = info.first;
	if (state == NO_STATE) return NO_STATE;	

	bool kingMove = kingCanMove(KingRank, KingFile, p);
	if(state == DOUBLECHECK)
	{
		if (kingMove) return CHECK;
		return MATE;
	} 
	//TODO comment on how to evaluate the checks
	if (state == CHECK)
	{
		// position of the piece that checked you
		square piece(info.second.first, info.second.second);
		int pieceType = abs(board[piece.first][piece.second]);

		square king = p == WHITE ? WhiteKing : BlackKing;

		// if the king can move or any piece can capture the attacking piece, it's not a mate
		if (kingMove || anyPieceCanCapture(piece.first, piece.second, p)) return CHECK;

		switch (pieceType){
			case BISHOP:
				if (canInterceptBishop(piece, king, p)) return CHECK;
				break;
			case ROOK:
				if (canInterceptRook(piece, king, p)) return CHECK;
				break;
			case QUEEN:
				if (king.first == piece.first || king.second == piece.second)
				{
					if (canInterceptRook(piece, king, p)) return CHECK;
				}
				if (king.first != piece.first && king.second != piece.second)
				{
					if (canInterceptBishop(piece, king, p)) return CHECK;
				}
			default: break;
		}

		// if you can't dodge or capture a knight, it's mate
		return MATE;
	}
	// if none of the above holds true, it's neither a check nor the king can move, thus it's a draw
	return STALEMATE;
}

checkInfo isCheck(int KingRank, int KingFile, player p)
{
	//TODO you don't need to check everything, you can only be attacked diagonally or horizon-/vertically 
	//TODO of the piece you just moved
	int checks = 0;

	// counts the amount of pieces that can currently walk to your king
	// it's -p because while we do check for the player, we check whether the OPPOSITE'S pieces could
	// technically take your king

	// we preinitialise these values because we'll return the position of the piece that is checking your king
	// if it's only one piece.
	int i = 0;
	int j = 0;
	for(; i < ranks; i++)
	{
		for(; j < files; j++)
		{
			if (sgn(board[i][j]) == -p && canMove(i, j, KingRank, KingFile, true, -p)) checks++;
		}		
	}
	if (checks == 0) return checkInfo(NO_STATE, square(0,0));
	if (checks == 1) return checkInfo(CHECK, square(i, j));
	return checkInfo(DOUBLECHECK, square(0, 0));
}

bool kingCanMove(int KingRank, int KingFile, player p)
{
	for(int i = -1; i < 1; i++)
	{
		for(int j = -1; j < 1; j++)
		{
			if (KingRank + i > 7 || KingRank + i < 0 || KingFile + j > 7 || KingFile + j < 0) continue;
			if (canMove(KingRank, KingFile, KingRank + i, KingFile + j, true, p)) return true;
		}
	}
	return false;
}

bool anyPieceCanCapture(int rank, int file, player p)
{
	for(int i = 0; i < ranks; i++)
	{
		for( int j = 0; j < files; j++)
		{
			if(sgn(board[i][j] == p))
			{
				if (canMove(i, j, rank, file, false, p)) return true;
			}
		}
	}
	return false;
}

bool anyPieceCanMove(int rank, int file, player p)
{
	for (int i = 0; i < ranks; i++)
	{
		for (int j = 0; j < files; j++)
		{
			if (sgn(board[i][j] == p))
			{
				if (canMove(i, j, rank, file, true, p)) return true;
			}
		}
	}
	return false;
}

bool canInterceptBishop(square piece, square king, player p)
{
	int vertDir = piece.first > king.first ? -1 : 1;
	int horDir = piece.second > king.second ? -1 : 1;

	// iterates to find out whether you can intercept the check
	for (int i = vertDir, j = horDir; abs(i) != abs(piece.first - king.first); i += vertDir, j += horDir)
	{
		if (anyPieceCanMove(i, j, p)) return true;
	}
	return false;
}

bool canInterceptRook(square piece, square king, player p)
{
	int vertDir = piece.first > king.first ? -1 : 1;
	int horDir = piece.second > king.second ? -1 : 1;

	if (king.first != piece.first)
	{
		for (int i = vertDir; abs(i) != abs(piece.first - king.first); i += vertDir)
		{
			if (anyPieceCanMove(i, king.second, p)) return true;
		}
	}
	else
	{
		for (int i = horDir; abs(i) != abs(piece.second - king.second); i += horDir)
		{
			if (anyPieceCanMove(king.first, i , p)) return true;
		}
	}
	return false;
}