#ifndef _AGENT_HPP
#define _AGENT_HPP
	#include "square.hpp"
	#include "board.hpp"
	#include <string.h>
	#include <string>
	#include <cstdio>
	#include <cstdlib>
	#include <random>
	#include <stack>
	#include <numeric>
	using std::string;
	using std::stack;
	using std::inner_product;

const double INF = 81000000.0;
const double MINF = -INF;
const double MIDDLE = (INF+MINF)/2, IINF = INF+1, MMINF = MINF-1;


typedef struct sucInform{
	double eval;
	vector<Square> moves;
}sucInform;

typedef enum AgentType{
	ALPHA_BETA=0, //"AB"
}AgentType;

int randInt(int N);
double randReal();
void printAgentType(const AgentType &which);
AgentType getAgentType(char *type);

class Player{
public:
	Square getMove(Board &board);
};

class Agent{
private:
	AgentType type;
	int depthLimit;
	string writeEvalName;
	bool isRand;
	double randRate;
	vector<double> priceTable;
	double (*evalFunc)(const Board &, const vector<double> &);
	
	static sucInform agentSearch(const Board &board, const int &depth, double alpha, double beta, bool warn, double (*const func)(const Board &, const vector<double> &), const vector<double> &table);
	double static ABEvalFunc(const Board &board, const vector<double> &table);
public:
	Agent(const AgentType &which, char *readFileName, char *writeFileName, int depthL, bool isRan, double rate);
	double evaluateBoard(const Board &board)const;
	Square getMove(const Board &board)const;
	void print()const;
	vector<double> getPriceTable()const;
	//void writePriceTable(int *array, double re)const;
};
#endif