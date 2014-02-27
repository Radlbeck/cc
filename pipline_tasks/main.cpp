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
#include <sys/wait.h>
using namespace std;

#define MAX_CMDS 5		//TODO upgrade to handle more commands
#define MAX_ARGS 5

int parse_user_commands(char *cmds[][MAX_ARGS], char *input, int *cmd_amount);
int child_first(char *cmd[], int pipefd[]);
int child_i(char *cmd[], int prev_pipe[], int next_pipe[]);

int main(int argc, char *argv[])
{
	// make sure correct parameter amount is entered
	if(argc != 2){												
		perror("Error: ./main [\"cmd | cmd\"]\n");		// inform user of usage
		perror("		  ^specify \"cmd\"s\n");		
		return -1;				                		// exit with an error
	}

	// parse user input
	int  cmd_amount, status;
	char *cmds[MAX_CMDS][MAX_ARGS];
	if(parse_user_commands(cmds, argv[1], &cmd_amount)) return -1;
		

	/*
	// setup pipe
	int pipefd[2];						// create pipe file descriptors
	if(pipe(pipefd) == -1){				// handle pipe errors
		perror("Error: could not open pipe\n");
		return -1;
	}
	*/
	int pipefd[(MAX_CMDS - 1)][2];			    // pipe amount = number of commands - 1 (cmd | cmd | cmd)
	for(int i = 0; i < (cmd_amount - 1); i++){	// only create the pipes needed
		if(pipe(pipefd[i]) == -1){			    // handle pipe errors
			perror("Error: could not open pipe\n");
			return -1;
		}
	}

	pid_t cid[(MAX_CMDS - 1)];
	printf("command amount: %d\n", cmd_amount); //TODO REMOVE
	for(int i = 0; i < (cmd_amount - 1); i++){	// last comand is executed in final parent
		cid[i] = fork();
		printf("%d -> %d\n", i, cid[i]); //TODO REMOVE
		if(cid[i] == -1)
		{ 
			perror("Error: could not create child\n"); 
			return -1; 
		}else if(cid[i] == 0){
			if(i == 0){
				printf("made it %d\n", i); //TODO remove
			       	child_first(cmds[i], pipefd[i]);
			}else{
				printf("made it? %d\n", i); //TODO remove
				child_i(cmds[i], pipefd[(i - 1)], pipefd[i]);
			}
		}else{
			printf("waiting for %d ...\n", cid[i]);	//TODO REMOVE
			pid_t t = wait(&status);
			printf("%d finished\n", t); //TODO REMOVE
		}
	}

	close(pipefd[(cmd_amount - 2)][1]);
	dup2(pipefd[(cmd_amount - 2)][0], 0);

	//TODO for bonus catch file redirection operators here in cmds[(cmd_amount - 1)][]
	execvp(cmds[(cmd_amount - 1)][0], cmds[(cmd_amount - 1)]);

	return 0;
}
	// setup process
	/*
	pid_t cid = fork();
	if(cid == -1){					// handle fork errors
		perror("Error: could not create child\n");
		return -1;	
	}
	

	if(cid == 0){		// in child
		child_first(cmds[0], pipefd);
	}else{			// in parent
		close(pipefd[1]);
		dup2(pipefd[0], 0);

		//TODO for bonus catch file redirection operators here in cmds[(cmd_amount - 1)][]
		execvp(cmds[1][0], cmds[1]);
	}
	

	return 0;
} 
*/

int child_first(char *cmd[], int pipefd[]){
	close(pipefd[0]); 		// close read end
	dup2(pipefd[1], 1);		// copys pipes write end into stdout [stdout > pipe]

	execvp(cmd[0], cmd);	// replace current procces with cmd	
	printf("Error: failed to launch %s\n", cmd[0]);	// if failed the code will continue
	return 0;
}

int child_i(char *cmd[], int prev_pipe[], int next_pipe[]){
	close(prev_pipe[1]);	// close prev out
	close(next_pipe[0]);	// close next in
	dup2(prev_pipe[0], 0);	// copys pipes read end into stdin [pipe < stdin]
	dup2(next_pipe[1], 1);	// copys pipes write end into stdout [stdout > pipe]	

	execvp(cmd[0], cmd);	// replace current procces with cmd
	printf("Error: failed to launch %s\n", cmd[0]);	// if failed the code will continue
	return 0;
}

int parse_user_commands(char *cmds[][MAX_ARGS], char *input, int *cmd_amount)
{	
	char *str, *token, *saveptr;
	char *sub_str, *sub_token, *sub_saveptr;	
	int i, j;
	for(i = 1, str = input; ; i++, str = NULL){ 	// 'str = NULL' - resume last spot of the string
		token = strtok_r(str, "|", &saveptr);	    // get pointer for next token
		if(token == NULL) break;		

		for(j = 1, sub_str = token; ;j++, sub_str=NULL){ 		// sub tokenize to find arguments for commands
			sub_token = strtok_r(sub_str, " ", &sub_saveptr);			

			if(i <= MAX_CMDS){					// only grab a pre definied amount of arguments and commands
				if(j <= (MAX_ARGS + 1)){
					cmds[(i - 1)][(j - 1)] = sub_token;			// store all cmds and args
					*cmd_amount = i;
					printf("%i, %i, %s\n", i, j, cmds[(i - 1)][(j - 1)]); //TODO REMOVE
				}else{
					perror("Error: too many arguments\n");
					return -1;
				}
			}else{
				perror("Error: too many commands\n");
					return -1;
			}
			if(sub_token == NULL) break;	// we want null in our argument array
		}				
	}
	return 0;
}
