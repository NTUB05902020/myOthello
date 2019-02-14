#include "agent.hpp"
#include "learn.hpp"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <ctime>
using std::thread;
using std::mutex;
using std::max_element;
using std::min_element;
using std::unique_lock;
using std::condition_variable;

#define THREAD_NUM 200
#define N 10000
const double ita = 100.0;

thread threads[THREAD_NUM];
int avaID; FILE *fp; time_t currentTime;
mutex fpLock, avaIDLock;
mutex cvM;
unique_lock<mutex> cvULM(cvM);
condition_variable cv;

void playGame(int threadID, const Agent &ag){
	Board brd;  int bNum, wNum; int result;
	vector<vector<double>> tmp;
	while(!brd.getLegalMoves().empty()){
		vector<double> tmp2 = brd.getBoardVec();
		tmp2.push_back(1.0); tmp.push_back(tmp2);
		brd = brd.getNextBoard(ag.getMove(brd));
	}
	vector<double> tmp2 = brd.getBoardVec(); tmp2.push_back(1.0);
	tmp.push_back(tmp2); brd.getNum(bNum, wNum);
	if(bNum > wNum) result = 1;
	else result = (wNum > bNum)? -1:0;
	fpLock.lock();
	for(int i=0;i<tmp.size();++i){
		for(int j=0;j<tmp[i].size();++j){
			fprintf(fp, "%d ", (int)tmp[i][j]);
		}
		fprintf(fp, "%d\n", result);
	}
	fpLock.unlock(); avaIDLock.lock(); avaID = threadID; cv.notify_all();
}

void printVec(const vector<double> &v){
	printf("vector:  ");
	for(auto i=v.begin();i!=v.end();++i) printf("%lf ", *i);
	printf("\n");
}

int main(int argc, char **argv){
	if(argc != 3){ printf("format: %s [evalFile] [writeTo]\n", argv[0]); exit(1);}
	//play game
	currentTime = time(NULL);
	printf("%s\n", ctime(&currentTime));
	Agent ag(CORNER, argv[1], argv[1], 5, true, 0.5);
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
	currentTime = time(NULL);
	printf("%s\n", ctime(&currentTime));
	fflush(fp); fclose(fp); exit(0);
}
