#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <iostream>

#define PAWN   1000
#define KNIGHT 3000
#define BISHOP 3001
#define ROOK   5000
#define QUEEN  9000
#define KING  25000

#define P PAWN
#define N KNIGHT
#define B BISHOP
#define R ROOK
#define Q QUEEN
#define K KING

#define WHITE  1
#define BLACK -1

const int ranks = 8;
const int files = 8;

typedef int position[ranks][files];

extern position board;

char associatedChar(int i);
void printBoard();
void setup();

#endif