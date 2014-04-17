/* CPU scheduler implementation using a multi level feedback queue
 */
#include <pthread.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <semaphore.h>

using namespace std;


void parse_arguments(char * infile)
{
	ifstream file (infile);								// open file into an input file stream
	if(file.is_open()){
		string word;
		while(file >> word){
			cout << word << endl;
		}
	}

}

int max_cpu_queue = 0;
int main(int argc, char * argv[])
{

	if(argc < 2) {cout << "Not enough arguments" << endl; exit(EXIT_FAILURE);}
	max_cpu_queue = atoi(argv[1]);
 	for(int i = 2; i < argc; i++){
 		parse_arguments(argv[i]);
 	}

	exit(EXIT_SUCCESS);
}
