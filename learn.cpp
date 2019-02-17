#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <thread>
#include <mutex>
#include <vector>
#include <array>
using std::thread;
using std::mutex;
using std::vector;
using std::array;
#define THREAD_NUM 200
#define TIMES 300
#define D 38

thread threads[THREAD_NUM];
mutex nowMutex, einMutex, graMutex;
time_t currentTime;
FILE *fp;
int N = 0, now = 0;
double ita=1000000.0, lr=0.0, log_2 = log(2), ein;
int order[D] = {0,1,2,5,6,7,8,9,10,13,14,15,16,17,18,21,22,23,40,41,42,45,46,47,48,49,50,53,54,55,56,57,58,61,62,63,64,65};
vector<array<double, D>> X;
vector<int> Y;
array<double, D> w, gra;

void printArray(array<double, D> a){
	for(int i=0;i<6;++i){
		for(int j=0;j<3;++j) printf("%lf ", a[i*6+j]);
		printf("     ");
		for(int j=3;j<6;++j) printf("%lf ", a[i*6+j]);
		printf("\n");
	}
	printf("%lf    %lf\n", a[36], a[37]);
}
 
double dot(array<double, D> a, array<double, D> b){
	double ret = 0.0;
	for(int i=0;i<D;++i) ret += a[i]*b[i];
	return ret;
}

void Ein(void){
	while(1){
		nowMutex.lock(); if(now == N){ nowMutex.unlock(); pthread_exit(0);}
		//printf("Ein thread %d  now = %d  ein = %lf\n", get_id(), now, ein);
		int me = now++; nowMutex.unlock();
		double tmp = (Y[me] == 0)? log_2 : log(1 + exp(-Y[me]*dot(w, X[me])));
		einMutex.lock(); ein += tmp; einMutex.unlock();
	}
}

void Gra(void){
	while(1){
		nowMutex.lock(); if(now == N){ nowMutex.unlock(); pthread_exit(0);}
		int me = now++; nowMutex.unlock();
		if(Y[me] == 0) continue;
		double tmp = dot(w, X[me]);
		tmp = (Y[me] > 0)? -1/(1 + exp(tmp)) : 1/(1 + exp(-tmp));
		graMutex.lock();
		for(int i=0;i<D;++i) gra[i] += X[me][i]*tmp;
		graMutex.unlock();
	}
}

int main(int argc, char **argv){
	if(argc != 5){ printf("format: %s [Kifu] [ReadEval] [WriteEval] [EIN record]\n", argv[0]); exit(1);}
	currentTime = time(NULL); printf("%s\n", ctime(&currentTime)); fflush(stdout);
	
	//read X Y
	if((fp = fopen(argv[1], "r")) == NULL){ printf("Failed to open %s\n", argv[1]); exit(1);}
	double tmp[66];
	while(fscanf(fp, "%lf", &tmp[0]) != EOF){
		int res; array<double, D> tmpArray;
		for(int i=1;i<66;++i) fscanf(fp, "%lf", &tmp[i]);
		for(int i=0;i<D;++i) tmpArray[i] = tmp[order[i]];
		fscanf(fp, "%d", &res); Y.push_back(res); X.push_back(tmpArray);
	}
	fclose(fp); N = X.size();
	
	//read w
	if((fp = fopen(argv[2], "rb")) == NULL){ printf("Failed to open %s\n", argv[2]); exit(1);}
	for(int i=0;i<D;++i) fread(&w[i], sizeof(double), 1, fp);
	fclose(fp);
	
	//start learn
	if((fp = fopen(argv[4], "w")) == NULL){ printf("Failed to open %s for EIN record\n", argv[3]); exit(1);}
	for(int i=0;i<TIMES;++i){
		ein = 0.0; now = 0;
		for(int j=0;j<THREAD_NUM;++j) threads[j] = thread(Ein);
		for(int j=0;j<THREAD_NUM;++j) threads[j].join();
		gra.fill(0.0); now = 0; lr += ein*ein;
		for(int j=0;j<THREAD_NUM;++j) threads[j] = thread(Gra);
		for(int j=0;j<THREAD_NUM;++j) threads[j].join();
		double tmp = ita/sqrt(lr);
		for(int j=0;j<D;++j){ gra[j] /= N; w[j] -= tmp*gra[j];}
		fprintf(fp, "%lf\n", ein);
		if(i%50 == 0){
			printf("times: %d  Ein: %lf\nW:\n", i, ein);
			printArray(w); printf("gra:\n"); printArray(gra); printf("\n");
		}
	}
	fflush(fp); fclose(fp);
	
	//write new w
	if((fp = fopen(argv[3], "wb")) == NULL){ printf("Failed to open %s for WriteEval\n", argv[2]); exit(1);}
	for(int i=0;i<D;++i) fwrite(&w[i], sizeof(double), 1, fp);
	fflush(fp); fclose(fp);
	currentTime = time(NULL); printf("%s\n", ctime(&currentTime)); fflush(stdout); exit(0);
}
