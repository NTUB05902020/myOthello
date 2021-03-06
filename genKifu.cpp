#include "agent.hpp"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <ctime>
using std::thread;
using std::mutex;
using std::unique_lock;
using std::condition_variable;

#define THREAD_NUM 200
#define N 6000

thread threads[THREAD_NUM];
int avaID; FILE *fp; time_t currentTime;
mutex fpLock, avaIDLock;
mutex cvM;
unique_lock<mutex> cvULM(cvM);
condition_variable cv;

void playGame(int threadID, const Agent &ag){
	Board brd;  int bNum, wNum; int result;
	vector<vector<double>> tmp;
	do{
		vector<double> tmp2 = brd.getBoardVec();
		brd.getNum(bNum, wNum); tmp2.push_back(bNum-wNum);
		tmp2.push_back(1.0); tmp.push_back(tmp2);
		brd = brd.getNextBoard(ag.getMove(brd));
	}while(!brd.getLegalMoves().empty());
	if(bNum > wNum) result = 1;
	else result = (wNum > bNum)? -1:0;
	fpLock.lock();
	for(int i=1;i<tmp.size();++i){
		for(int j=0;j<tmp[i].size();++j){
			fprintf(fp, "%d ", (int)tmp[i][j]);
		}
		fprintf(fp, "%d\n", result); fflush(fp);
	}
	fpLock.unlock(); avaIDLock.lock(); avaID = threadID; cv.notify_all();
}

int main(int argc, char **argv){
	if(argc != 3){ printf("format: %s [evalFile] [writeTo]\n", argv[0]); exit(1);}
	//play game
	currentTime = time(NULL); printf("%s\n", ctime(&currentTime)); fflush(stdout);
	Agent ag(CORNER, argv[1], argv[1], 5, true, 0.4);
	if((fp = fopen(argv[2], "w")) == NULL){ printf("Failed to open %s\n", argv[2]); exit(1);}
	int called = (N < THREAD_NUM)? N:THREAD_NUM, done = 0;
	for(int i=0;i<called;++i) threads[i] = thread(playGame, i, ag);
	for(;called < N;++called){
		cv.wait(cvULM); threads[avaID].join(); ++done;
		threads[avaID] = thread(playGame, avaID, ag); avaIDLock.unlock();
	}
	while(done < called){
		cv.wait(cvULM); threads[avaID].join();
		avaIDLock.unlock(); ++done;
	}
	currentTime = time(NULL); printf("%s\n", ctime(&currentTime)); fflush(stdout);
	fclose(fp); exit(0);
}
