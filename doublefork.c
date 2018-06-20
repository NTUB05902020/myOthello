#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

const char *output = "output.txt";

int main(){
	int c_pid, gc_pid, fd = open(output, O_RDWR|O_SYNC|O_CREAT);
	if((c_pid = vfork()) < 0){
		printf("Failed to vfork\n");
		exit(1);
	}else if(c_pid > 0){
		//parent
		printf("child pid == %d\n", c_pid);
		if((gc_pid = fork()) < 0){
			printf("Failed to fork\n");
			exit(1);
		}else if(gc_pid > 0){
			//child
			printf("grand_child pid == %d\n", gc_pid);
			_exit(0);
		}else{
			//grand_child
			if(fd < 0){
				printf("Failed to open %s\n", output);
				exit(1);
			}
			if(dup2(fd, STDOUT_FILENO) < 0){
				printf("Failed to dup2 stdout\n");
				exit(1);
			}
			if(dup2(fd, STDERR_FILENO) < 0){
				printf("Failed to dup2 stderr\n");
				exit(1);
			}
			execlp("./learn", "./learn", NULL);
		}
	}
	exit(0);
}
