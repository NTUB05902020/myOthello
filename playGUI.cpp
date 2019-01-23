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
	while(1){
		if(board.getLegalMoves().empty()){
			board.reverseTurn();
			if(board.getLegalMoves().empty()) break;
		}
		board.getBoardString(buffer); write(pipe_fd[1], buffer, 65);
		move = (board.isBlacksTurn())? b.getMove(board) : w.getMove(board);
	}
	board.print(true);
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
		close(pipe_fd[0]); buffer[64] = '\n'; buffer[65] = '\0';
		char blackIn[32], whiteIn[32]; int bDepth, wDepth;
		//set game information
		printf("black: [Type] [in] [depth]\nwhite: [Type] [in] [depth]\n");
		scanf("%s%s%d", whiteIn, blackIn, &bDepth);
		Agent b(getAgentType(whiteIn), blackIn, blackIn, bDepth, false, 0.0);
		scanf("%s%s%d", blackIn, whiteIn, &wDepth);
		Agent w(getAgentType(blackIn), whiteIn, whiteIn, wDepth, false, 0.0);
		b.print(); w.print(); playGame(b, w); kill(GUI_pid, 9);
	}
	exit(0);
}