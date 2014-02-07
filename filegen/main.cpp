// Code for basic file manipulation
//
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

char initfile()
{
	ofstream myfile ("test");
	if(myfile.is_open()){
		myfile << " ";
		myfile.close();
	}else{
		cout << "cannot create file" << endl;
		return 0;
	}
	return 1;
}

char addstr(string s, int size)
{
	fstream myfile ("test");
	myfile.seekg(0, ios::end);		// sets the cursor to the eof
	if(myfile.is_open()){
		while( myfile.tellg() < size ){	//.tellg() returns the amount of bytes from the cursor position
			myfile << s;
		}
		myfile.close();
	}else{
		cout << "cannot open file" << endl;
		return 0;
	}
	return 1;
}

int filesize()
{
	ifstream myfile ("test", ios::binary | ios::ate);
	int size = (int) myfile.tellg();
	myfile.close();

	return size;
}

int main()
{
	if(initfile())
		addstr("U™U™U™U™U™U™U™U™U™U™U™", 4194304);

	cout << filesize() << endl;

	return 0;
}

