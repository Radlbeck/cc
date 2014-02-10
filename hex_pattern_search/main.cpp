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
	int pattern_aa = 0xaa;
	int pattern_55 = 0x55;
	int count_aa = 0;
	int count_55 = 0;

	pfile = fopen(argv[1], "r");

	if(pfile != NULL){
		while(current_bit != EOF){
			current_bit = fgetc(pfile);
			if(current_bit == pattern_aa)
				count_aa++;
			if(current_bit == pattern_55)
				count_55++;
		}
		fclose(pfile);
	}else{
		cout << "File cannot be opened" << endl;
		return 1;
	}

	cout << "count 0x55: " << count_55 << endl;
	cout << "count 0xaa: " << count_aa << endl;

	return 0;
}

