#ifndef _SQUARE_HPP
#define _SQUARE_HPP
	#include <cstdio>

typedef enum Square{
	OUT_OF_BOUND = -1,
	S11 , S12, S13, S14, S15, S16, S17, S18,
	S21, S22, S23, S24, S25, S26, S27, S28,
	S31, S32, S33, S34, S35, S36, S37, S38,
	S41, S42, S43, S44, S45, S46, S47, S48,
	S51, S52, S53, S54, S55, S56, S57, S58,
	S61, S62, S63, S64, S65, S66, S67, S68,
	S71, S72, S73, S74, S75, S76, S77, S78,
	S81, S82, S83, S84, S85, S86, S87, S88
}Square;

typedef enum Direction{
	Right=0, RightUp, Up, LeftUp,
	Left, LeftDown, Down, RightDown
}Direction;

const int di[8] = {0, -1, -1, -1, 0, 1, 1, 1},
	  dj[8] = {1, 1, 0, -1, -1, -1, 0, 1};

bool atLeft(Square square);
bool atRight(Square square);
bool atUp(Square square);
bool atDown(Square square);
bool legalNextSquare(Square recent, Direction direction);
Square getSquare(int i, int j);
int getI(Square square);
int getJ(Square square);
bool outOfBound(int i, int j);
bool onDirection(Square start, Square end, Direction direction);
void printSquare(Square square);
Square nextSquare(Square recent, Direction direction);
#endif
