/*
 *  Lab 1 - Basic multiprogramming
 *
 *	By: Andrew Radlbeck & John Wood
 */
#include <cstdio>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
using namespace std;

#define MAX_CMDS 2		//TODO upgrade to handle more commands
#define MAX_BUF  1024   //TODO I have no idea if this is enough

int main(int argc, char *argv[])
{
	// make sure correct parameter amount is entered
	if(argc != 2){												
		printf("Error: %s [\"cmd | cmd\"]\n", argv[0]);		// inform user of usage
		printf("		  ^specify \"cmd\"s\n");		
		return -1;										    // exit with an error
	}

	// get cmds from user input
	char *str, *token, *saveptr;		//TODO sub tokenize to handle command arguments
	char *cmds[MAX_CMDS];
	int i;
	for(i = 1, str = argv[1]; ; i++, str = NULL){ 	// 'str = NULL' - resume last spot of the string
		token = strtok_r(str, "|", &saveptr);		// get pointer for next token
		if(token == NULL) break;		

		//TODO optimize - consider only moving pointers instead of allocating more memory MUST for args to work
		int j, k;
		int len = strlen(token);
		char *token_trim = new char;
		for(j = 0, k = 0; j < len; j++){
			if(!isspace(token[j])){						
				token_trim[k++] = token[j];				
			}
		}

		// save commands into an array
		if(i <= MAX_CMDS){
			cmds[(i - 1)] = token_trim;			
		}else{
			printf("Error: too many commands\n");
			printf("\t\t MAX COMMANDS = %i\n", MAX_CMDS);
			return -1;
		}
	}
	printf("%s, %s\n", cmds[0], cmds[1]); //TODO REMOVE

	// setup process
	int pipefd[2];			// create pipe file descripters
	pid_t cid;

	if(pipe(pipefd) == -1){							// handle pipe errors
		printf("Error: could not open pipe\n");
		return -1;
	}

	cid = fork();
	if(cid == -1){									// handle fork errors
		printf("Error: could not create child\n");
		return -1;	
	}

	if(cid == 0){	// in child
		close(pipefd[0]); 		// close read end
		dup2(pipefd[1], 1);		// copys pipes write end into stdout [stdout > pipe]

		execlp(cmds[0], cmds[0], NULL);						// replace child with cmd		
		printf("Error: failed to launch %s\n", cmds[0]);	// if failed the code will continue
	}else{			// in parent
		close(pipefd[1]); 		// close write end

		char buf;	// reads bit-by-bit from pipe to stdout
		while(read(pipefd[0], &buf, 1)){
			write(1, &buf, 1);
		}printf("\n");

		close(pipefd[0]);		// close pipe
	}

	return 0;
} 
