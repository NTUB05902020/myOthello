#ifndef _AGENT_HPP
#define _AGENT_HPP
	#include "square.hpp"
	#include "board.hpp"
	#include <string>
	#include <stdio.h>
	#include <stdlib.h>
	using std::string;

char defaultFileName[5] = "eval";
typedef enum AgentType{
	PLAYER = 0, ALPHA_BETA, ALPHA_BETA_RAND
}AgentType;

void printAgentType(AgentType type);

class Agent{
private:
	AgentType type;
	string readEvalName, writeEvalName;
	double priceTable[64];
	
	void setEvalNames(char *a, char *b);
	void getPriceTable();
public:
	Agent(const AgentType &which, char *readFileName);
	Agent(const AgentType &which, char *readFileName, char *writeFileName);
	void print();
	double evaluateBoard(const Board &board);
	//Square getBestMove(const Board &board);
};

#endif
