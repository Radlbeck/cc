// Code for basic file search
//
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main(int argc, char* argv[])
{
	// get input file
	if(argc != 2){	
		cout << "No input file specified" << endl;
		return 1;
	}

	//TODO parse file into hex (use hexdump?)
	//system("hexdump -C" + argv[1] + "> dumped"); 


	// print/search file contents
	ifstream file (argv[1]);
	string line;
	string pat1 = "55";
	string pat2 = "aa";

	if(file.is_open()){
		while(getline (file,line)){
			size_t found = line.find(pat1);
			if(found != string::npos){
				found = line.find(pat2);
				if(found != string::npos){
					cout << "found:" << line << endl;
				}
			}
		}
		file.close();
	}else{
		cout << "File cannot be opened" << endl;
		return 1;
	}

	return 0;
}

