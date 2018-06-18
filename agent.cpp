#include "agent.hpp"

char defaultFileName[5] = "eval";
std::default_random_engine generator;

void printAgentType(AgentType type){
	switch(type){
		case PLAYER: printf("type: PLAYER\n"); break;
		case ALPHA_BETA: printf("type: ALPHA_BETA\n"); break;
		case ALPHA_BETA_RAND: printf("type: ALPHA_BETA_RAND\n"); break;
		default: printf("type: DEFAULT\n");
	}
}

int randInt(int N){
	std::uniform_int_distribution<int> distribution(0, N-1);
	return distribution(generator);
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

sucInform Agent::alphaBeta(const Board &board, double alpha, double beta, const int &depth, bool warn){
	sucInform ret;
	if(depth == depthLimit){
		ret.eval = evaluateBoard(board); return ret;
	}
	Board tempBoard(board); vector<Square> legalMoves = tempBoard.getLegalMoves();
	if(legalMoves.empty()){
		if(warn){
			bitset<64> black = board.getAllBlack(), white = board.getAllWhite();
			int bNum = black.count(), wNum = white.count();
			if(bNum > wNum) ret.eval = INF-1;
			else ret.eval = (bNum < wNum)? MINF+1 : 0.0;
			return ret;
		}else{
			tempBoard.reverseTurn();
			return alphaBeta(tempBoard, alpha, beta, depth, true);
		}
	}
	int n=legalMoves.size(); sucInform childInform;
	if(tempBoard.isBlacksTurn()){
		double max = MINF;
		for(int i=0;i<n;++i){
			Board nextBoard(tempBoard); nextBoard.changeBoard(legalMoves[i]);
			childInform = alphaBeta(nextBoard, alpha, beta, depth+1, false);
			if(childInform.eval > max){
				ret.eval = max = childInform.eval;
				ret.moves.clear(); ret.moves.push_back(legalMoves[i]);
			}else if(childInform.eval == max)
				ret.moves.push_back(legalMoves[i]);
			if(childInform.eval > alpha){
				alpha =  childInform.eval;
				if(beta <= alpha) break;
			}
		}
	}else{
		double min = INF;
		for(int i=0;i<n;++i){
			Board nextBoard(tempBoard); nextBoard.changeBoard(legalMoves[i]);
			childInform = alphaBeta(nextBoard, alpha, beta, depth+1, false);
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

Agent::Agent(const AgentType &which = PLAYER, char *readFileName = NULL, int depthL = 5){
	if((type = which) != PLAYER){
		depthLimit = depthL;
		setEvalNames(readFileName, readFileName);
		getPriceTable();
	}
}

Agent::Agent(const AgentType &which, char *readFileName, char *writeFileName, int depthL){
	type = which; depthLimit = depthL;
	setEvalNames(readFileName, writeFileName);
	getPriceTable();
}

void Agent::print(){
	printAgentType(type);
	if(type != PLAYER) printf("read: %s   write: %s  depth limit = %d\n", readEvalName.c_str(), writeEvalName.c_str(), depthLimit);
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

Square Agent::getBestMove(const Board &board){
	sucInform result; int num;
	switch(type){
		case ALPHA_BETA:
			result = alphaBeta(board, MINF, INF, 0, false);
			if((num = result.moves.size()) == 0){ printf("alphaBeta returned empty moves!\n"); exit(1);}
			return result.moves[0];
		case ALPHA_BETA_RAND:
			result = alphaBeta(board, MINF, INF, 0, false);
			if((num = result.moves.size()) == 0){ printf("alphaBeta returned empty moves!\n"); exit(1);}
			return result.moves[randInt(num)];
	}
}
/*

int main(){
	Agent agent3(ALPHA_BETA_RAND, "eval3");
	agent3.print();
	Board board;
	for(int i=0;i<16;++i){
		printSquare(agent3.getBestMove(board));
	}
	return 0;
}
*/
