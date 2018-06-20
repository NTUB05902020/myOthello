#include "board.hpp"
#include "agent.hpp"
#include <cstdio>

void clearScreen(int n){
	for(int i=0;i<n;++i) printf("\n");
}

int main(){
	int i, j; Square move;
	Agent agent2(ALPHA_BETA, "eval3", "eval3w", 9);
	Agent agent1(ALPHA_BETA_RAND, "eval4", "eval4w", 9);
	agent1.print(); agent2.print();
	int blackwin = 0, whitewin = 0, draw = 0;
	for(int k=0;k<10;++k){
		Board board;
		board.print();
		while(!board.isGameEnded()){
			if(!board.haveLegalMove()){
				board.reverseTurn();
				if(!board.haveLegalMove()) board.setGameEnded();
				continue;
			}
			//board.print();
			if(board.isBlacksTurn()){
				/*scanf("%d%d", &i, &j);
				move = getSquare(i-1, j-1);
				*/
				move = agent1.getBestMove(board);
			}else
				move = agent2.getBestMove(board);
			//printSquare(move);
			//printf("%s\n", (board.changeBoard(move))? "":"Invalid move\n");
			board.changeBoard(move);
		}
		printf("k = %d\n", k);
		board.print();
	}
	return 0;
}
