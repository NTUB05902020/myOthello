#ifndef _METHOD_HPP
#define _METHOD_HPP
	#include "square.hpp"
	#include "board.hpp"
const double INF = 81000000.0;
const double MINF = -INF;
int depthLimit = 7;

typedef struct successorInformation{
	double eval;
	vector<Square> moves;
}sucInform;

sucInform alphaBeta(const Agent &agent, const Board &board, double alpha, double beta, const int &depth, bool warn){
	sucInform ret;
	if(depth == depthLimit){
		ret.eval = agent.evaluateBoard(board); return ret;
	}
	Board tempBoard(board); vector<Square> legalMoves = tempBoard.getLegalMoves();
	if(legalMoves.empty()){
		if(warn){
			bitset<64> black = board.getAllBlack(), white = board.getAllWhite();
			int bNum = black.count(), wNum = white.count();
			if(bNum > wNum) ret = INF-1;
			else ret = (bNum < wNum)? MINF+1 : 0.0;
			return ret;
		}else{
			tempBoard.reversBoard();
			return alphaBeta(agent, tempBoard, alpha, beta, depth, true);
		}
	}
	if(board.isBlacksTurn()){
		double max = MINF;
		
		
		
	}else{
		
		
		
		
		
	}
}
#endif
