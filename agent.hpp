#ifndef _AGENT_HPP
#define _AGENT_HPP
	#include "square.hpp"
	#include "board.hpp"
	#include <string>
	#include <stdio.h>
	#include <stdlib.h>
	#include <random>
	using std::string;

const double INF = 81000000.0;
const double MINF = -INF;

typedef enum AgentType{
	PLAYER = 0, ALPHA_BETA, ALPHA_BETA_RAND
}AgentType;

typedef struct successorInformation{
	double eval;
	vector<Square> moves;
}sucInform;

AgentType readAgentType();
void printAgentType(AgentType type);
int randInt(int N);
double randReal();

class Agent{
private:
	AgentType type;
	int depthLimit;
	string readEvalName, writeEvalName;
	double priceTable[64], rand;
	
	void setEvalNames(char *a, char *b);
	void getPriceTable();
	sucInform alphaBeta(const Board &board, double alpha, double beta, const int &depth, bool warn);
public:
	Agent();
	Agent(const AgentType &which, char *readFileName, int depthL, double ran);
	Agent(const AgentType &which, char *readFileName, char *writeFileName, int depthL, double ran);
	void print();
	void writePriceTable(unsigned int *array, double re);
	double evaluateBoard(const Board &board);
	Square getBestMove(Board &board);
};

#endif
