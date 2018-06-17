#include "agent.hpp"

void printAgentType(AgentType type){
	switch(type){
		case PLAYER: printf("type: PLAYER\n"); break;
		case ALPHA_BETA: printf("type: ALPHA_BETA\n"); break;
		case ALPHA_BETA_RAND: printf("type: ALPHA_BETA_RAND\n"); break;
		default: printf("type: DEFAULT\n");
	}
}

void Agent::setEvalNames(char *a, char *b){
	readEvalName = a; writeEvalName = b;
}

void Agent::getPriceTable(){
	const char *str = readEvalName.c_str();	FILE *fp = fopen(str, "r");
	if(fp == NULL){ printf("Failed to open %s\n", str); exit(1);}
	double *p = priceTable;
	for(int i=0;i<64;i+=8) fscanf(fp, "%lf%lf%lf%lf%lf%lf%lf%lf", &p[i], &p[i+1], &p[i+2], &p[i+3], &p[i+4], &p[i+5], &p[i+6], &p[i+7]);
}

Agent::Agent(const AgentType &which = PLAYER, char *readFileName = NULL){
	if((type = which) != PLAYER){
		setEvalNames(readFileName, readFileName);
		getPriceTable();
	}
}

Agent::Agent(const AgentType &which, char *readFileName, char *writeFileName){
	type = which;
	setEvalNames(readFileName, writeFileName);
	getPriceTable();
}

void Agent::print(){
	printAgentType(type);
	printf("read: %s   write: %s\n", readEvalName.c_str(), writeEvalName.c_str());
	//print price_table?
}

double Agent::evaluateBoard(const Board &board){
	double ret = 0.0;
	bitset<64> black = board.getAllBlack(), white = board.getAllWhite();
	for(int i=0;i<64;++i){
		if(white[i]) ret -= priceTable[i];
		else if(black[i]) ret += priceTable[i];
	}
	return ret;
}

/*Square Agent::getBestMove(const Board &board){
	switch(type){
		case ALPHA_BETA:
			
			
			return ;
		case ALPHA_BETA_RAND:
			
			
			
			return ;
	}
}
*/

int main(){
	printf("Hello agent.cpp\n");
	Agent agent;
	agent.print();
	Agent agent2(PLAYER, "eval");
	agent2.print();
	Agent agent3(ALPHA_BETA, "eval3");
	agent3.print();
	Agent agent4(ALPHA_BETA, "eval4", "eval4w");
	agent4.print();
	return 0;
}
