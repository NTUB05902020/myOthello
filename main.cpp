#include "board.hpp"
#include <cstdio>

void clearScreen(int n){
	for(int i=0;i<n;++i) printf("\n");
}

int main(){
	Board board; int i, j;
	while(!board.isGameEnded()){
		board.print();
		scanf("%d%d", &i, &j);
		printf("%s\n", (board.changeBoard(getSquare(i-1, j-1)))? "":"Invalid move\n");
	}
	return 0;
}
