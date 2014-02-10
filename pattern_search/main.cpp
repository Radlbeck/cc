// Code for basic file search
//
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main(int argc, char* argv[])
{
	if(argc != 2){
		cout << "No input file specified" << endl;
		return 1;
	}

	ifstream file (argv[1]);
	string line;
	if(file.is_open()){
		while(getline (file,line)){
			cout << line << endl;
		}
		file.close();
	}else{
		cout << "File cannot be opened" << endl;
		return 1;
	}

	return 0;
}

