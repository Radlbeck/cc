/*
 *  Lab 1 - Basic multiprogramming
 *
 *	By: Andrew Radlbeck & John Wood
 *
 *
 * TODO:
 *			-- change errors to redirect to stderr
 *			-- close all pipe ends that are not being used in the current process
 *
 */
#include <cstdio>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;

#define MAX_CMDS 2		//TODO upgrade to handle more commands
#define MAX_ARGS 5

int parse_user_commands(char *cmds[][MAX_ARGS], char *input, int *cmd_amount, int *arg_amount);
int child_first(char *cmd[], int pipefd[]);
int child_last(char *cmd[], int next_pipe[], int prev_pipe[]);

int main(int argc, char *argv[])
{
	// make sure correct parameter amount is entered
	if(argc != 2){												
		printf("Error: %s [\"cmd | cmd\"]\n", argv[0]);		// inform user of usage
		printf("		  ^specify \"cmd\"s\n");		
		return -1;										    // exit with an error
	}

	// parse user input
	int  cmd_amount, arg_amount;
	char *cmds[MAX_CMDS][MAX_ARGS];
	if(parse_user_commands(cmds, argv[1], &cmd_amount, &arg_amount)) return -1;
		

	// setup process
	int pipefd[2];		// create pipe file descripters
	int pipefd_b[2];		// create pipe file descripters	

	if(pipe(pipefd) == -1){							// handle pipe errors
		printf("Error: could not open pipe\n");
		return -1;
	}

	if(pipe(pipefd_b) == -1){							// handle pipe errors
		printf("Error: could not open pipe\n");
		return -1;
	}

	pid_t cid = fork();
	if(cid == -1){									// handle fork errors
		printf("Error: could not create child\n");
		return -1;	
	}

	if(cid == 0){	// in child
		child_first(cmds[0], pipefd);

	}else{			// in parent
		waitpid(cid, 0, 0);
		pid_t cid1 = fork();

		if(cid1 == -1){									// handle fork errors
			printf("Error: could not create child\n");
			return -1;	
		}

		if(cid1 == 0){
			child_last(cmds[1], pipefd_b, pipefd);
		}else{

			close(pipefd_b[1]);	 		// close write end
			close(pipefd[0]);
			close(pipefd[1]);			
			char buf;	// reads bit-by-bit from pipe to stdout

			while(read(pipefd_b[0], &buf, 1)){				
				write(1, &buf, 1);				
			}printf("\n");	
			//kill(cid1, SIGINT); //TODO need to return the signal back to the child process somehow
			wait(NULL);
			//waitpid(cid1, 0, 0);	
			close(pipefd_b[0]);		// close pipe
			
		}				
	}

	return 0;
} 

int child_first(char *cmd[], int pipefd[]){
	close(pipefd[0]); 		// close read end
	dup2(pipefd[1], 1);		// copys pipes write end into stdout [stdout > pipe]

	execvp(cmd[0], cmd);								// replace child with cmd		//TODO this needs to be execvp
	printf("Error: failed to launch %s\n", cmd[0]);	// if failed the code will continue
	return 0;
}

int child_last(char *cmd[], int next_pipe[], int prev_pipe[]){
	close(prev_pipe[1]);
	close(next_pipe[0]);
	dup2(prev_pipe[0], 0);		// copys pipes read end into stdin [pipe < stdin]
	dup2(next_pipe[1], 1);		// copys pipes write end into stdout [stdout > pipe]	
	dup2(next_pipe[0], 0);		// copys pipes write end into stdout [stdout > pipe]

	execvp(cmd[0], cmd);								// replace child with cmd		//TODO this needs to be execvp
	printf("Error: failed to launch %s\n", cmd[0]);	// if failed the code will continue
	return 0;
}

int parse_user_commands(char *cmds[][MAX_ARGS], char *input, int *cmd_amount, int *arg_amount)
{
	// TODO to add bonous part 2 insert another tokenizer to catch ">,<,<<,>>"
	//	- add file redirection code to pipe as well
	// get cmds from user input
	char *str, *token, *saveptr;					//TODO sub tokenize to handle command arguments
	char *sub_str, *sub_token, *sub_saveptr;	
	int i, j;
	for(i = 1, str = input; ; i++, str = NULL){ 	// 'str = NULL' - resume last spot of the string
		token = strtok_r(str, "|", &saveptr);		// get pointer for next token
		if(token == NULL) break;		

		for(j = 1, sub_str = token; ;j++, sub_str=NULL){ 		// sub tokenize to find arguments for commands
			sub_token = strtok_r(sub_str, " ", &sub_saveptr);			

			if(i <= MAX_CMDS){
				if(j <= (MAX_ARGS + 1)){
					cmds[(i - 1)][(j - 1)] = sub_token;
					//printf("%i, %i, %s\n", i, j, cmds[(i - 1)][(j - 1)]); //TODO REMOVE
				}else{
					printf("Error: too many arguments\n");
					return -1;
				}
			}else{
				printf("Error: too many commands\n");
					return -1;
			}
			if(sub_token == NULL) break;					// we want the null in our argument array
		}				
	}
	cmd_amount = &i;
	arg_amount = &j;

	return 0;
}