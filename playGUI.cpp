#include "agent.hpp"
#include "board.hpp"
#include <unistd.h>
#include <cstdio>
#include <errno.h>
#include <sys/types.h>
#include <signal.h>

char buffer[66];
int pipe_fd[2];
char mes1[32] = "Failed to create pipe_fd: ",
		   mes2[32] = "Failed to fork: ",
		   mes3[32] = "Child failed to dup2 STDIN: ";

void perror_exit(char *message, int value){
	perror(message); exit(value);
}

void child_perror_exit(char *message, int value){
	perror(message); kill(getppid(), 9); exit(value);
}


void parent_perror_exit(char *message, pid_t child_pid, int value){
	perror(message); kill(child_pid, 9); exit(1);
}

void playGame(Agent &b, Agent &w){
	Board board; Square move;
	while(!board.isGameEnded()){
		if(!board.haveLegalMove()){
			board.reverseTurn();
			if(!board.haveLegalMove()) board.setGameEnded();
			continue;
		}
		board.getBoardString(buffer); write(pipe_fd[1], buffer, 65);
		move = (board.isBlacksTurn())? b.getMove(board) : w.getMove(board);
		//printSquare(move);
		/*printf((*/board.changeBoard(move)/*)? "true":"false")*/;
	}
	board.print();
}

int main(){
	int GUI_pid;
	if(pipe(pipe_fd) == -1) perror_exit(mes1, 1);
	if((GUI_pid = fork()) < 0) perror_exit(mes2, 1);
	else if(GUI_pid == 0){
		close(pipe_fd[1]);
		if(dup2(pipe_fd[0], STDIN_FILENO) == -1) child_perror_exit(mes3, 1);
		execlp("python3", "python3", "GUI.py", NULL);
	}else{
		printf("parent child: %d %d\n", getpid(), GUI_pid);
		close(pipe_fd[0]); AgentType bT, wT;
		buffer[64] = '\n'; buffer[65] = '\0';
		char blackIn[32], whiteIn[32];
		int bDepth, wDepth; double bRand, wRand;
		//set game information
		printf("black: [Type] [in] [depth]\nwhite: [Type] [in] [depth]\n");
		bT = readAgentType(); scanf("%s%d", blackIn, &bDepth);
		wT = readAgentType(); scanf("%s%d", whiteIn, &wDepth);
		Agent b(bT, blackIn, bDepth, 1.0), w(wT, whiteIn, wDepth, 1.0);
		b.print(); w.print();
		playGame(b, w); kill(GUI_pid, 9);
	}
	exit(0);
}
