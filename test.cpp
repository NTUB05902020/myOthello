#include "agent.hpp"
#include "learn.hpp"
#include <thread>
#include <mutex>
using std::thread;
using std::mutex;
using std::max_element;
using std::min_element;

#define THREAD_NUM 100
#define N 2000
#define TIMES 100
const double ita = 10000.0;

thread threads[THREAD_NUM];
int job = 0;
vector<vector<double>> X;
vector<int> Y;
mutex XLock, YLock;
char buffer[8] = "ab.eval";
Agent ag(ALPHA_BETA, buffer, buffer, 5, true, 0.3);

void playGame(){
	Board brd;  int bNum, wNum; int result;
	while(!brd.getLegalMoves().empty()) brd = brd.getNextBoard(ag.getMove(brd));
	brd.getNum(bNum, wNum);
	if(bNum > wNum) result = 1;
	else result = (wNum > bNum)? -1:0;
	XLock.lock(); vector<double> tmp = brd.getBoardVec();
	tmp.push_back(1.0); X.push_back(tmp); XLock.unlock();
	YLock.lock();  Y.push_back(result);  YLock.unlock();
}


int main(int argc, char **argv){
	for(int hi=0;hi<50;++hi){
		for(;job<N;job+=THREAD_NUM){
			for(int j=0;j<THREAD_NUM;++j)
				threads[j] = thread(playGame);
			for(int j=0;j<THREAD_NUM;++j)
				threads[j].join();
			printf("jobs = %d\n", job);
		}
		vector<double> w = ag.getPriceTable();
		FILE *fp1 = fopen("Ein_record", "a");
		FILE *fp2 = fopen("ab.eval", "wb");
		for(int i=0;i<TIMES;++i){
			double res = Ein(w, X, Y);
			fprintf(fp1, "%lf\n", res);
			vector<double> tmp = scalMul(graEin(w, X, Y), -ita);
			/*for(auto i=tmp.begin();i!=tmp.end();++i)
				printf("%lf ", *i);
			printf("\n");
			*/
			w = vecAdd(w, tmp);
			double M = -1, m = 0;
			for(auto i=w.begin();i!=w.end();++i){
				double val = (*i < 0)? (-*i):(*i);
				if(M < val) M = val;
				if(m > val) m = val;
			}
			if(m > 0) w = scalDiv(w, m);
			else if(M > 0) w = scalDiv(w, M);
			/*for(auto i=w.begin();i!=w.end();++i)
				printf("%lf ", *i);
			printf("\n");
			*/if(i % 20 == 0) printf("learning times: %d\n", i);
		}
		for(int i=0;i<65;++i) fwrite(&w[i], sizeof(double), 1, fp2);
		fclose(fp1); fclose(fp2);
	}
	exit(0);
}
