/*
 *  Lab 1 - Basic multiprogramming
 *
 *	By: Andrew Radlbeck & John Wood
 */
#include <cstdio>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
using namespace std;

int main(int argc, char* argv[])
{
	if(argc != 2){										// make sure correct parameter amount is entered
		printf("Error: %s [ls/emacs]\n", argv[0]);		// if not inform user of usage
		printf("		  ^specify cmd\n");		
		return -1;										// exit with an error
	}
	char* cmd = argv[1]; //	save cmd
	pid_t cid = fork();  // create child

	if(cid == 0){		 // is child process
		sleep(300);		 
		execlp(cmd, cmd, NULL);   // replace current procces with 'cmd'
		_exit(0);		          // exit: _exit() for child
	}else{				 // is parent process
		sleep(200);		 
		exit(0);		 
	}

	return 0;
} 
