/*
 *  Lab 1 - Basic multiprogramming
 *
 *	By: Andrew Radlbeck & John Wood
 */
#include <cstdio>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
using namespace std;

#define MAX_CMDS 2		//TODO upgrade to handle more commands

int main(int argc, char *argv[])
{
	// make sure correct parameter amount is entered
	if(argc != 2){												
		printf("Error: %s [\"cmd | cmd\"]\n", argv[0]);		// inform user of usage
		printf("		  ^specify \"cmd\"s\n");		
		return -1;										    // exit with an error
	}

	// get cmds from user input
	char *str, *token, *saveptr;
	char *cmds[MAX_CMDS];
	int i;
	for(i = 1, str = argv[1]; ; i++, str = NULL){ 	// 'str = NULL' - resume last spot of the string
		token = strtok_r(str, "|", &saveptr);		// get pointer for next token
		if(token == NULL) break;		

		//TODO optimize - consider only moving pointers instead of allocation more memory
		int j, k;
		int len = strlen(token);
		char *token_trim = new char;
		for(j = 0, k = 0; j < len; j++){
			if(!isspace(token[j])){						
				token_trim[k++] = token[j];				
			}
		}

		// save comands into an array
		if(i <= MAX_CMDS){
			cmds[(i - 1)] = token_trim;			
		}else{
			printf("Error: too many commands\n");
			printf("\t\t MAX COMMANDS = %i\n", MAX_CMDS);
			return -1;
		}
	}
	printf("%s, %s\n", cmds[0], cmds[1]); //TODO REMOVE

	// launch processes
	

	return 0;
} 
