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
#define N 600
#define TIMES 120
const double ita = 100.0;

thread threads[THREAD_NUM];
int avaID;
mutex XYLock, avaIDLock;
char buffer[8] = "LI.eval";
vector<vector<double>> X;
vector<int> Y;
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
	XYLock.lock(); int addNum = tmp.size();
	for(int i=0;i<addNum;++i){
		X.push_back(tmp[i]); Y.push_back(result);
	}
	XYLock.unlock(); avaIDLock.lock(); avaID = threadID; cv.notify_all();
}

void printVec(const vector<double> &v){
	printf("vector:  ");
	for(auto i=v.begin();i!=v.end();++i) printf("%lf ", *i);
	printf("\n");
}

int main(int argc, char **argv){
	for(int hi=0;hi<12;++hi){
		printf("hi = %d\n", hi);
		//play game
		X.clear(); Y.clear();
		Agent ag(CORNER, buffer, buffer, 7, true, 0.5);
		int called = (N < THREAD_NUM)? N:THREAD_NUM, done = 0;
		for(int i=0;i<called;++i) threads[i] = thread(playGame, i, ag);
		for(;called < N;++called){
			cv.wait(cvULM); threads[avaID].join();
			if((++done) % 100 == 0) printf("  done = %d\n", done);
			threads[avaID] = thread(playGame, avaID, ag); avaIDLock.unlock();
		}
		while(done < called){
			cv.wait(cvULM); threads[avaID].join(); avaIDLock.unlock();
			if((++done) % 100 == 0) printf("  done = %d\n", done);
		}
		//start learn
		vector<double> w = ag.getPriceTable();
		printf("    "); printVec(w);
		time_t my_time = time(NULL); printf("  time:  %s\n", ctime(&my_time));
		for(int i=0;i<TIMES;++i){
			double res = Ein(w, X, Y);
			vector<double> tmp = scalMul(graEin(w, X, Y), -ita);
			w = vecAdd(w, tmp);
			if(i % 20 == 0) printf("  learning times: %d\n", i);
		}
		FILE *fp = fopen("LI.eval", "wb");  int wLength = w.size();
		for(int i=0;i<wLength;++i) fwrite(&w[i], sizeof(double), 1, fp);
		fflush(fp); fclose(fp);
		if(hi % 3 == 0){
			char outputFile[32]; sprintf(outputFile, "eval/%d.eval", hi/3);
			fp = fopen(outputFile, "wb");
			for(int i=0;i<65;++i) fwrite(&w[i], sizeof(double), 1, fp);
			fflush(fp); fclose(fp);
		}
	}
	exit(0);
}
