#include "board.hpp"
#include "agent.hpp"

#define SIZE 32

char bType[SIZE], wType[SIZE],
	blackIn[SIZE], blackOut[SIZE],
	whiteIn[SIZE], whiteOut[SIZE];

void playGame(Agent &b, Agent &w, unsigned int *record){
	Board board; Square move;
	while(!board.isGameEnded()){
		if(!board.haveLegalMove()){
			board.reverseTurn();
			if(!board.haveLegalMove()) board.setGameEnded();
			continue;
		}
		if(board.isBlacksTurn()){
			move = b.getBestMove(board); ++(record[move]);
		}else{
			move = w.getBestMove(board); --(record[move]);
		}
		board.changeBoard(move);
	}
}


int main(){
	unsigned int game_per_time, times;
	int bDepth, wDepth, blackLearn; AgentType bT, wT;
	double bRand, wRand, reward;
	printf("black: [Type] [In] [Out] [Depth] [Rand]\nwhite: [Type] [In] [Out] [Depth] [Rand]\n[game/time] [times] [black learn?] [reward]\n");
	bT = readAgentType(); scanf("%s%s%d%lf", blackIn, blackOut, &bDepth, &bRand);
	wT = readAgentType(); scanf("%s%s%d%lf", whiteIn, whiteOut, &wDepth, &wRand);
	scanf("%u%u%d%lf", &game_per_time, &times, &blackLearn, &reward);
	Agent bAgent(bT, blackIn, blackOut, bDepth, bRand), wAgent(wT, whiteIn, whiteOut, wDepth, wRand);
	unsigned int record[64]; Agent *whoLearn = (blackLearn)? &bAgent : &wAgent;
	for(int i=0;i<64;++i) record[i] = game_per_time;
	
	for(unsigned int i=0;i<times;++i){
		for(unsigned int j=0;j<game_per_time;++j) playGame(bAgent, wAgent, record);
		printf("%u time over\n", i);
		whoLearn->writePriceTable(record, reward);
	}
	return 0;
}
