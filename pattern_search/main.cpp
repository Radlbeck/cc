// Code for basic file search
//
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main(int argc, char* argv[])
{
	if(argc != 2){						// get file name
		cout << "No input file specified" << endl;
		return 1;
	}

	//TODO parse file into hex (use hexdump?)


	ifstream file (argv[1], ios::binary);			// print/search file contents
	string line;
	if(file.is_open()){
		while(getline (file,line)){	//TODO find patters in 'line'
			cout << line << endl;
		}
		file.close();
	}else{
		cout << "File cannot be opened" << endl;
		return 1;
	}

	return 0;
}

