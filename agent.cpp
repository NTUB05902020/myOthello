#include "agent.hpp"

char defaultFileName[5] = "eval";
std::default_random_engine generator;
char buf[32];


int randInt(int N){
	std::uniform_int_distribution<int> distribution(0, N-1);
	return distribution(generator);
}

double randReal(){
	std::uniform_real_distribution<double> distribution(0.0, 1.0);
	return distribution(generator);
}

void printAgentType(const AgentType &which){
	printf("Type: ");
	switch(which){
		case NAIIVE: printf("NA\n"); break;
		case LINEAR: printf("LI\n"); break;
	}
}

AgentType getAgentType(char *type){
	if(strcmp(type, "NA") == 0) return NAIIVE;
	else if(strcmp(type, "LI") == 0) return LINEAR;
	else{ printf("No such type!\n"); exit(1);}
}

Square Player::getMove(Board &board){
	vector<Square> legalMoves = board.getLegalMoves();
	int num = legalMoves.size(), iRead, jRead;
	for(int i=0;i<num;++i) printf("(%d %d) ", getI(legalMoves[i])+1, getJ(legalMoves[i])+1);
	while(1){
		scanf("%d%d", &iRead, &jRead); --iRead; --jRead;
		if(!outOfBound(iRead, jRead)){
			Square ret = getSquare(iRead, jRead);
			if(!board.canReverse(ret).empty()) return ret;
		}
		printf("Invalid move!\n");
	}
}

sucInform Agent::agentSearch(const Board &board, const int &depth, double alpha, double beta, bool warn, double (*const func)(const Board &, const vector<double> &), const vector<double> &table){
	sucInform ret;
	if(depth == 0){ ret.eval = func(board, table); return ret;}
	vector<Square> legalMoves = board.getLegalMoves();
	if(legalMoves.empty()){
		if(warn){
			int bNum, wNum; board.getNum(bNum, wNum);
			if(bNum > wNum) ret.eval = INF;
			else ret.eval = (bNum < wNum)? MINF:MIDDLE;
			return ret;
		}
		Board tmpBoard(board); tmpBoard.reverseTurn(); return agentSearch(board, depth, alpha, beta, true, func, table);
	}
	int n=legalMoves.size(); sucInform childInform;
	if(board.isBlacksTurn()){
		double max = MMINF;
		for(int i=0;i<n;++i){
			childInform = agentSearch(board.getNextBoard(legalMoves[i]), depth-1, alpha, beta, false, func, table);
			if(childInform.eval > max){
				ret.eval = max = childInform.eval;
				ret.moves.clear(); ret.moves.push_back(legalMoves[i]);
			}else if(childInform.eval == max)
				ret.moves.push_back(legalMoves[i]);
			if(childInform.eval > alpha){
				alpha = childInform.eval;
				if(beta <= alpha) break;
			}
		}
	}else{
		double min = IINF;
		for(int i=0;i<n;++i){
			childInform = agentSearch(board.getNextBoard(legalMoves[i]), depth-1, alpha, beta, false, func, table);
			if(childInform.eval < min){
				ret.eval = min = childInform.eval;
				ret.moves.clear(); ret.moves.push_back(legalMoves[i]);
			}else if(childInform.eval == min)
				ret.moves.push_back(legalMoves[i]);
			if(childInform.eval < beta){
				beta = childInform.eval;
				if(beta <= alpha) break;
			}
		}
	}
	return ret;
}

double Agent::NAEvalFunc(const Board &board, const vector<double> &table){
	vector<double> tmp = board.getBoardVec();
	return accumulate(tmp.begin(), tmp.end(), 0);
}

double Agent::LIEvalFunc(const Board &board, const vector<double> &table){
	vector<double> tmp = board.getBoardVec();
	return inner_product(tmp.begin(), tmp.end(), table.begin(), table[64]);
}

Agent::Agent(const AgentType &which, char *readFileName, char *writeFileName=NULL, int depthL=5, bool isRan=false, double rate=0.3){
	type = which; depthLimit = depthL; randRate = ((isRand = isRan))? rate:-1.0;
	if(writeFileName != NULL) writeEvalName = string(writeFileName);
	double tmp; FILE *fp;
	switch(which){
		case NAIIVE:
			evalFunc = NAEvalFunc; break;
		case LINEAR:
			fp = fopen(readFileName, "rb");
			if(fp == NULL){ printf("Failed to open %s\n", readFileName); exit(1);}
			for(int i=0;i<65;++i){
				fread(&tmp, sizeof(double), 1, fp);
				priceTable.push_back(tmp);
			}
			evalFunc = LIEvalFunc; fclose(fp); break;
	}
}

double Agent::evaluateBoard(const Board &board)const{
	return evalFunc(board, priceTable);
}

Square Agent::getMove(const Board &board)const{
	if(isRand && randReal() <= randRate){
		vector<Square> tmp = board.getLegalMoves();
		return tmp[randInt(tmp.size())];
	}else{
		sucInform tmp = agentSearch(board, depthLimit, MMINF, IINF, false, evalFunc, priceTable);
		return tmp.moves[randInt(tmp.moves.size())];
	}
}

void Agent::print()const{
	printAgentType(type);
	printf("DepthLimit: %d\nIsRand: %d\nRandRate: %lf\nwrite to: %s\n", depthLimit, isRand, randRate, writeEvalName.c_str());
}

vector<double> Agent::getPriceTable()const{
	if(type == NAIIVE){ printf("try to get PriceTable of NAIIVE\n"); exit(1);}
	return priceTable;
}