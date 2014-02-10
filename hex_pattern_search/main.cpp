// Code for basic file search
//
#include <cstdio>
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
	// get input file
	if(argc != 2){	
		cout << "No input file specified" << endl;
		return 1;
	}

	// print/search file contents
	FILE * pfile;
	int current_bit = 0;
	int pattern_count_aa = 0;
	int pattern_count_55 = 0;

	pfile = fopen(argv[1], "r");

	if(pfile != NULL){
		while(current_bit != EOF){
			current_bit = fgetc(pfile);
			cout << current_bit << endl;
		}
		fclose(pfile);
	}else{
		cout << "File cannot be opened" << endl;
		return 1;
	}

	return 0;
}

