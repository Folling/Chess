#ifndef MOVE_VAL_H
#define MOVE_VAL_H

#include "board.h"
#include <conio.h>

#define NO_STATE 0
#define CHECK 1
#define DOUBLECHECK 2
#define MATE 3
#define STALEMATE 4

typedef short state;
typedef short player;
typedef std::pair<int, int> square;
typedef std::pair<state, square> checkInfo;

static square WhiteKing(0, 4);
static square BlackKing(7, 4);

// to generalize the pieces (black pieces and white pieces)
int abs(int i);
// to check which side a piece belongs to (returns 0 for empty squares)
int sgn(int i);

// takes the current position and the goal as an input and validates the move
bool canMove(int fromRank, int fromFile, int toRank, int toFile, bool checked, player p);

// checks whether there is a piece inbetween, for the movement of bishops, rooks and queens
bool pieceInbetween(int fromRank, int fromFile, int toRank, int toFile, int piece);

// these functions just validate the moves so that canMove()'s switch doesn't get clustered

bool checkPawn(int fromRank, int fromFile, int toRank, int toFile, bool checkForCheck);
bool checkKnight(int fromRank, int fromFile, int toRank, int toFile);
bool checkBishop(int fromRank, int fromFile, int toRank, int toFile);
bool checkRook(int fromRank, int fromFile, int toRank, int toFile);
bool checkQueen(int fromRank, int fromFile, int toRank, int toFile);
bool checkKing(int fromRank, int fromFile, int toRank, int toFile, player p);

state checkState(int KingRank, int KingFile, player p);

// returns true if the player is currently in check and what type of check it is
// (double or single check)
checkInfo isCheck(int KingRank, int KingFile, player p);
// returns true if the king can't move anymore
bool kingCanMove(int KingRank, int KingFile, player p);
// returns true if any piece of the player specified can move to the square
bool anyPieceCanCapture(int rank, int file, player p);
// returns true if any piece of the player specified can move to the square
bool anyPieceCanMove(int rank, int file, player p);

bool canInterceptBishop(square piece, square king, player p);
bool canInterceptRook(square piece, square king, player p);


#endif