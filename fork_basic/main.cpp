/*
 *  Pre-Lab 1 - Basic multiprogramming
 *
 *	By: Andrew Radlbeck & John Wood
 */
#include <cstdio>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
using namespace std;

int main(int argc, char * argv[])
{

	pid_t cid = fork();  // create child

	if(cid == 0){		 // is child process
		sleep(300);		 
		_exit(0);		 // exit: _exit() for child
	}else{				 // is parent process
		sleep(200);		 
		exit(0);		 
	}
	return 0;
} 