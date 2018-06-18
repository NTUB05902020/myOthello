#ifndef _BOARD_HPP
#define _BOARD_HPP
	#include "square.hpp"
	#include <bitset>
	#include <cstdio>
	#include <cstdlib>
	#include <vector>
	using std::bitset;
	using std::vector;

class Board{
private:
	bool ended, blacksTurn;
	bitset<64> occupied, white;
	int isWho(Square square)const;

public:
	Board();
	Board(const Board &another);
	void setGameEnded();
	void reverseTurn();
	bool isGameEnded()const;
	bool isBlacksTurn()const;
	bool isBlack(Square square)const;
	bool isWhite(Square square)const;
	bool isEmpty(Square square)const;
	bitset<64> getAllEmpty()const;
	bitset<64> getAllBlack()const;
	bitset<64> getAllWhite()const;
	void setBlack(Square square);
	void setWhite(Square square);
	void setEmpty(Square square);
	vector<Direction> canReverse(Square square);
	bool haveLegalMove();
	vector<Square> getLegalMoves();
	void print()const;
	bool changeBoard(Square square);
};
#endif
