#include "board.hpp"
#include "agent.hpp"
#include <mutex>
#include <thread>
using std::move;
using std::thread;
using std::mutex;
#define SIZE 32
#define THREAD_NUMBER 16

#define PRINT(ARGS...) do{printf(ARGS);fflush(stdout);}while(0)

char bType[SIZE], wType[SIZE], blackIn[SIZE],
	blackOut[SIZE], whiteIn[SIZE], whiteOut[SIZE];
unsigned int game_per_time, times, job;
int bDepth, wDepth, blackLearn; AgentType bT, wT;
double bRand, wRand, reward;
mutex jobLock, recordLock;
thread threads[THREAD_NUMBER];
const int deltaRecord[2][2] = {{-1, 1}, {1, -1}};

void playGame(Agent b, Agent w, int *record, int threadId){
	while(1){
		jobLock.lock();
		if(job == game_per_time){ jobLock.unlock(); break;}
		++job; jobLock.unlock();
		Board board; Square move;
		while(!board.isGameEnded()){
			if(!board.haveLegalMove()){
				board.reverseTurn();
				if(!board.haveLegalMove()) board.setGameEnded();
				continue;
			}
			if(board.isBlacksTurn()){
				move = b.getMove(board); recordLock.lock();
				record[move] += deltaRecord[blackLearn][0]; recordLock.unlock();
			}else{
				move = w.getMove(board); recordLock.lock();
				record[move] += deltaRecord[blackLearn][1]; recordLock.unlock();
			}
			board.changeBoard(move);
		}
	}
}

int main(){
	printf("black: [Type] [In] [Out] [Depth] [Rand]\nwhite: [Type] [In] [Out] [Depth] [Rand]\n[game/time] [times] [black learn?] [reward]\n");
	bT = readAgentType(); scanf("%s%s%d%lf", blackIn, blackOut, &bDepth, &bRand);
	wT = readAgentType(); scanf("%s%s%d%lf", whiteIn, whiteOut, &wDepth, &wRand);
	scanf("%u%u%d%lf", &game_per_time, &times, &blackLearn, &reward);
	Agent bAgent(bT, blackIn, blackOut, bDepth, bRand), wAgent(wT, whiteIn, whiteOut, wDepth, wRand);
	blackLearn = (blackLearn)? 1:0; int record[64]; Agent *whoLearn = (blackLearn)? &bAgent : &wAgent;
	
	for(unsigned int i=0U;i<times;++i){
		job = 0U;
		for(int k=0;k<64;++k) record[k] = game_per_time;
		for(int k=0;k<THREAD_NUMBER;++k){
			threads[k] = thread(playGame, bAgent, wAgent, record, k);
		}
		for(int k=0;k<THREAD_NUMBER;++k) threads[k].join();
		PRINT("%u time over\n", i);
		whoLearn->writePriceTable(record, reward);
	}
	return 0;
}
