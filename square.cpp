#include "square.hpp"
#include <cstdlib>

bool atLeft(Square square){
	return (square % 8 == 0);
}

bool atRight(Square square){
	return (square % 8 == 7);
}

bool atUp(Square square){
	return (square < 8);
}

bool atDown(Square square){
	return (square > 55);
}

bool legalNextSquare(Square recent, Direction direction){
	if(atLeft(recent) && (direction == LeftUp || direction == Left || direction == LeftDown)) return false;
	if(atRight(recent) && (direction == RightDown || direction == Right || direction == RightUp)) return false;
	if(atUp(recent) && (direction == RightUp || direction == Up || direction == LeftUp)) return false;
	if(atDown(recent) && (direction == LeftDown || direction == Down || direction == RightDown)) return false;
	return true;
}

int getI(Square square){
	return (square / 8);
}

int getJ(Square square){
	return (square % 8);
}

Square getSquare(int i, int j){
	return (Square)(i*8 + j);
}

bool outOfBound(int i, int j){
	return (i < 0 || i > 7 || j < 0 || j > 7)? true:false;
}

bool onDirection(Square start, Square end, Direction direction){
	if(start == end) return true;
	int si = getI(start), sj = getJ(start),
		ei = getI(end)  , ej = getJ(end);
	switch(direction){
		case 0: //Right
			return (ei == si && ej > sj);
		case 1: //RightUp
			return (ei < si && (si-ei) == (ej-sj));
		case 2: //Up
			return (ej == sj && ei < si);
		case 3: //LeftUp
			return (ei < si && (si-ei) == (sj-ej));
		case 4: //Left
			return (ei == si && ej < sj);
		case 5: //LeftDown
			return (ei > si && (ei-si) == (sj-ej));
		case 6: //Down
			return (ej == sj && ei > si);
		case 7: //RightDown
			return (ei > si && (ei-si) == (ej-sj));
		default: //No Such Direction
			printf("Failed at onDirection:  No such direction!\n"); exit(1); 
	}
}

Square nextSquare(Square recent, Direction direction){
	int newi = getI(recent) + di[direction], newj = getJ(recent) + dj[direction];
	return (outOfBound(newi, newj))? OUT_OF_BOUND : getSquare(newi, newj);
}

void printSquare(Square square){
	printf("%d %d\n", getI(square)+1, getJ(square)+1);
}
