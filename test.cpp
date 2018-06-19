#include "board.hpp"
#include "agent.hpp"
#include <cstdio>

void clearScreen(int n){
	for(int i=0;i<n;++i) printf("\n");
}

int main(){
	int i, j; Square move;
	Agent agent1, agent2;
	agent1.print(); agent2.print();
	int blackwin = 0, whitewin = 0, draw = 0;
	Board board;
	while(!board.isGameEnded()){
		if(!board.haveLegalMove()){
			board.reverseTurn();
			if(!board.haveLegalMove()) board.setGameEnded();
			continue;
		}
		board.print();
		move = (board.isBlacksTurn())? agent1.getMove(board) : agent2.getMove(board);
		printSquare(move);
		printf((board.changeBoard(move))? "true":"false");
	}
	board.print();
	return 0;
}
