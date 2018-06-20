#include "board.hpp"
#include "agent.hpp"
#include <pthread.h>
#include <mutex>
using namespace std;
#define SIZE 32
#define THREAD_NUMBER 16
struct Arg{
  Agent bAgent, wAgent;
  int *record, id;
};
char bType[SIZE], wType[SIZE],
        blackIn[SIZE], blackOut[SIZE],
	whiteIn[SIZE], whiteOut[SIZE];
unsigned int game_per_time, times;
int bDepth, wDepth, blackLearn; AgentType bT, wT;
double bRand, wRand, reward;
pthread_t threadId[THREAD_NUMBER];
unsigned int job;
mutex jobLock, recordLock;
static void *playGame(void *ARG){
  Arg *arg = (Arg *)ARG;
  Agent b = arg->bAgent, w = arg->wAgent;
  int *record = arg->record;
  while(1){
    jobLock.lock();
    if(job == game_per_time){ jobLock.unlock(); break; }
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
	++(record[move]); recordLock.unlock();
	
      }else{
	move = w.getMove(board); recordLock.lock();
	--(record[move]); recordLock.unlock();
      }
      board.changeBoard(move);
    }
  }
  pthread_exit(0);
}

int main(){
  printf("black: [Type] [In] [Out] [Depth] [Rand]\nwhite: [Type] [In] [Out] [Depth] [Rand]\n[game/time] [times] [black learn?] [reward]\n");
  bT = readAgentType(); scanf("%s%s%d%lf", blackIn, blackOut, &bDepth, &bRand);
  wT = readAgentType(); scanf("%s%s%d%lf", whiteIn, whiteOut, &wDepth, &wRand);
  scanf("%u%u%d%lf", &game_per_time, &times, &blackLearn, &reward);
  Agent bAgent(true, bT, blackIn, blackOut, bDepth, bRand), wAgent(false, wT, whiteIn, whiteOut, wDepth, wRand);
  int record[64]; Agent *whoLearn = (blackLearn)? &bAgent : &wAgent;
  struct Arg arg[THREAD_NUMBER];
  for(int i = 0;i<THREAD_NUMBER;++i) arg[i] = {bAgent, wAgent, record, i};
  for(unsigned int i=0;i<times;++i){
    for(int k=0;k<64;++k) record[k] = game_per_time;
    job = 0U;
    for(int k=0;k<THREAD_NUMBER;++k){
      pthread_create(&threadId[k], NULL, playGame, &arg[k]);
    }
    void *status;
    for(int k=0;k<THREAD_NUMBER;++k) pthread_join(threadId[k], &status);
    printf("%u time over\n", i); fflush(stdout);
    whoLearn->writePriceTable(record, ((blackLearn)? reward:-reward));
  }
  return 0;
}
