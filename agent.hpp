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

void printAgentType(AgentType type);
int randInt(int N);

class Agent{
private:
	AgentType type;
	int depthLimit;
	string readEvalName, writeEvalName;
	double priceTable[64];
	
	void setEvalNames(char *a, char *b);
	void getPriceTable();
	sucInform alphaBeta(const Board &board, double alpha, double beta, const int &depth, bool warn);
public:
	Agent(const AgentType &which, char *readFileName, int depthL);
	Agent(const AgentType &which, char *readFileName, char *writeFileName, int depthL);
	void print();
	double evaluateBoard(const Board &board);
	Square getBestMove(const Board &board);
};

#endif
