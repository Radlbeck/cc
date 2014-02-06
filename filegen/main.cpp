// Code for basic file manipulation
//


#include <iostream>
#include <fstream>

using namespace std;

int main()
{
	ofstream myfile;
	myfile.open("test");
	if(myfile.is_open()){
		myfile << "HIIIIIII\n";
		myfile.close();
	}else{
		cout << "Error: Cannot open file";
	}

	return 0;
}

int get_file_size(ofstream f)
{
	streampos begin, end;
	f.seekg(0, )



}
