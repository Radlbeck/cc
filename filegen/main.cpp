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

char addstr(string s, int x)
{
	fstream myfile ("test");
	myfile.seekg(0, ios::end);
	if(myfile.is_open()){
		while( myfile.tellg() < x ){
			myfile << s;
		}
		myfile.close();
	}else{
		cout << "Cannot open file" << endl;
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
	initfile();	
	addstr("ddddddddddd", 4194304);

	cout << filesize() << endl;

	return 0;
}

