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
	void reverseTurn();
	bool isBlacksTurn()const;
	bool isBlack(Square square)const;
	bool isWhite(Square square)const;
	bool isEmpty(Square square)const;
	void getBoardString(char *string)const;
	vector<double> getBoardVec()const;
	bitset<64> getAllEmpty()const;
	bitset<64> getAllBlack()const;
	bitset<64> getAllWhite()const;
	void getNum(int &bNum, int &wNum)const;
	void setBlack(Square square);
	void setWhite(Square square);
	void setEmpty(Square square);
	vector<Direction> canReverse(Square square)const;
	vector<Square> getLegalMoves()const;
	void print(bool ended)const;
	Board getNextBoard(Square square)const;
};
#endif
