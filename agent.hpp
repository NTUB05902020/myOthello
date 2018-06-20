#ifndef _AGENT_HPP
#define _AGENT_HPP
	#include "square.hpp"
	#include "board.hpp"
	#include <string>
	#include <stdio.h>
	#include <stdlib.h>
	#include <random>
//#include <functional>
	using std::string;
//using std::function;

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
	bool isBlack;
	int depthLimit;
	string readEvalName, writeEvalName;
	double priceTable[64], rand;
  //function<Square(Board &)> getMoveFunction;
	
	void setEvalNames(char *a, char *b);
	void getPriceTable();
	sucInform alphaBeta(const Board &board, double alpha, double beta, const int &depth, bool warn);
	Square getBestMove(Board &board);
	Square playerGetMove(Board &board);
public:
	Agent();
	Agent(bool isB, const AgentType &which, char *readFileName, int depthL, double ran);
	Agent(bool isB, const AgentType &which, char *readFileName, char *writeFileName, int depthL, double ran);
	void print();
	void writePriceTable(int *array, double re);
	double evaluateBoard(const Board &board);
	Square getMove(Board &board);
};

#endif
