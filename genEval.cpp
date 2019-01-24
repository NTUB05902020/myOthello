#include "agent.hpp"
#include "learn.hpp"
#include <thread>
#include <mutex>
#include <queue>
using std::thread;
using std::mutex;
using std::max_element;
using std::min_element;
using std::queue;

#define THREAD_NUM 200
#define N 1000
#define TIMES 120
const double ita = 100.0;

thread threads[THREAD_NUM];
mutex XLock, YLock;
char buffer[8] = "LI.eval";
vector<vector<double>> X;
vector<int> Y;
queue<int> thread_pool;

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
	XLock.lock(); YLock.lock();
	int addNum = tmp.size(); addNum = (addNum<9)?addNum:9;
	for(auto i=tmp.end()-addNum;i!=tmp.end();++i) X.push_back(*i);
	for(int i=0;i<addNum;++i) Y.push_back(result);
	XLock.unlock();	YLock.unlock();
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
		Agent ag(LINEAR, buffer, buffer, 7, true, 0.5);
		thread_pool = queue<int>();
		int job = (N < THREAD_NUM)? N:THREAD_NUM;
		for(int i=0;i<job;++i){
			threads[i] = thread(playGame, i, ag);
			thread_pool.push(i);
		}
		while(1){
			int jobLeft = N - job; if(jobLeft <= 0) break;
			int available = thread_pool.size();
			for(int i=0;i<jobLeft;++i){
				int popped = thread_pool.front();
				threads[popped].join(); ++job;
				threads[popped] = thread(playGame, popped, ag);
				if(job % 100 == 0) printf("  job = %d\n", job);
				thread_pool.pop(); thread_pool.push(popped);
			}
		}
		job = thread_pool.size();
		for(int i=0;i<job;++i){
			threads[thread_pool.front()].join();
			thread_pool.pop();
		}
		//start learn
		vector<double> w = ag.getPriceTable();
		printf("    "); printVec(w);
		FILE *fp2 = fopen("LI.eval", "wb");
		for(int i=0;i<TIMES;++i){
			double res = Ein(w, X, Y);
			vector<double> tmp = scalMul(graEin(w, X, Y), -ita);
			w = vecAdd(w, tmp);
			double M = -1, m = 0;
			for(auto i=w.begin();i!=w.end();++i){
				double val = (*i < 0)? (-*i):(*i);
				if(M < val) M = val;
				if(m > val) m = val;
			}
			if(m > 0) w = scalDiv(w, m);
			else if(M > 0) w = scalDiv(w, M);
			if(i % 20 == 0) printf("  learning times: %d\n", i);
		}
		for(int i=0;i<65;++i) fwrite(&w[i], sizeof(double), 1, fp2);
		fflush(fp2); fclose(fp2);
		if(hi % 3 == 0){
			char outputFile[32]; sprintf(outputFile, "eval/%d.eval", hi/3);
			FILE *fp = fopen(outputFile, "wb");
			for(int i=0;i<65;++i) fwrite(&w[i], sizeof(double), 1, fp);
			fflush(fp); fclose(fp);
		}
	}
	exit(0);
}
