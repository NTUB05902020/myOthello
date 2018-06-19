#include "board.hpp"

int Board::isWho(Square square)const{
	return (occupied[square])? ((white[square])? 2: 1) : 0;
}

Board::Board(){
	ended = false; blacksTurn = true;
	occupied.reset(); white.reset();
	setWhite(S44); setWhite(S55);
	setBlack(S45); setBlack(S54);
}

Board::Board(const Board &another){
	*this = another;
}

void Board::setGameEnded(){
	ended = true;
}

void Board::reverseTurn(){
	blacksTurn = !blacksTurn;
}

bool Board::isGameEnded()const{
	return ended;
}

bool Board::isBlacksTurn()const{
	return blacksTurn;
}

bool Board::isBlack(Square square)const{
	return (occupied[square] && !white[square]);
}

bool Board::isWhite(Square square)const{
	return (occupied[square] && white[square]);
}

bool Board::isEmpty(Square square)const{
	return !occupied[square];
}

void Board::getBoardString(char *string)const{
	for(int i=0;i<64;++i)
		string[i] = '0'+isWho((Square)i);
}

bitset<64> Board::getAllEmpty()const{
	bitset<64> temp = occupied; temp.flip();
	return temp;
}

bitset<64> Board::getAllBlack()const{
	bitset<64> temp = white; temp.flip();
	return (occupied & (temp));
}

bitset<64> Board::getAllWhite()const{
	return (occupied & white);
}

void Board::setBlack(Square square){
	occupied.set(square);
	white.reset(square);
}

void Board::setWhite(Square square){
	occupied.set(square);
	white.set(square);
}

void Board::setEmpty(Square square){
	occupied.reset(square);
	white.reset(square);
}

vector<Direction> Board::canReverse(Square square){
	vector<Direction> ret; bool flag;
	if(!isEmpty(square)) return ret;
	if(blacksTurn){
		for(Direction direction = Right;((int)direction)<8;direction = (Direction)(direction+1)){
			flag = false; Square recent = square;
			while(1){
				recent = nextSquare(recent, direction);
				if(recent != OUT_OF_BOUND){
					if(isWhite(recent)){
						flag = true; continue;
					}else if(isBlack(recent) && flag){
						ret.push_back(direction);
					}
				}
				break;
			}
		}
	}else{
		for(Direction direction = Right;((int)direction)<8;direction = (Direction)(direction+1)){
			flag = false; Square recent = square;
			while(1){
				recent = nextSquare(recent, direction);
				if(recent != OUT_OF_BOUND){
					if(isBlack(recent)){
						flag = true; continue;
					}else if(isWhite(recent) && flag){
						ret.push_back(direction);
					}
				}
				break;
			}
		}
	}
	return ret;
}

bool Board::haveLegalMove(){
	vector<Direction> dirs;
	for(int i=0;i<64;++i){
		dirs = canReverse((Square)i);
		if(!dirs.empty()) return true;
	}
	return false;
}

vector<Square> Board::getLegalMoves(){
	vector<Square> ret; vector<Direction> dirs;
	for(int i=0;i<64;++i){
		dirs = canReverse((Square)i);
		if(!dirs.empty()) ret.push_back((Square)i);
	}
	return ret;
}

void Board::print()const{
	if(ended){
		printf("Game Ended!\n");
		bitset<64> black = getAllBlack(), white = getAllWhite();
		int bn = black.count(), wn = white.count();
		printf("Black: %d   White: %d\n", bn, wn);
		if(bn > wn) printf("Black won!\n");
		else if(bn == wn) printf("Draw QQ\n");
		else printf("White won!\n");
	}else{
		printf("%s's Turn\n", (blacksTurn)? "Black":"White");
		for(int i=0, who=-1;i<8;++i){
			for(int j=0;j<8;++j) printf("%d ", isWho((Square)++who));
			printf("\n");
		}
	}
}

bool Board::changeBoard(Square square){
	vector<Direction> dirs = canReverse(square);
	if(dirs.empty()) return false;
	int length = dirs.size();
	if(blacksTurn){
		setBlack(square);
		for(int i=0;i<length;++i){
			Square recent = nextSquare(square, dirs[i]);
			while(isWhite(recent)){
				setBlack(recent);
				recent = nextSquare(recent, dirs[i]);
			}
		}		
	}else{
		setWhite(square);
		for(int i=0;i<length;++i){
			Square recent = nextSquare(square, dirs[i]);;
			while(isBlack(recent)){
				setWhite(recent);
				recent = nextSquare(recent, dirs[i]);
			}
		}
	}
	reverseTurn();
	return true;
}
