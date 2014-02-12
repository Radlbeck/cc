// Code for basic data manipulation
//
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
using namespace std;

#define ERROR        "Error: "
#define MAX_WORD_LEN 100

struct node{				// node data structure for DLL
	char word[MAX_WORD_LEN];
	node * next;
	node * prev;
};

/*node**/int getWords(char * infile)
{
	ifstream file (infile);								// open file into an input file stream
	if(file.is_open()){
		string word;
		while(file >> word){							// stream operator parses on white space (seperating words)
			transform(word.begin(), word.end(), word.begin(), ::tolower);	// make sure all words are lower case
			cout << word << endl;
		}
		file.close();
	}else{
		cout << ERROR << "cannot open file" << endl;
		return 0;
	}
	return -1;
}

int main(int argc, char * argv[])
{
	// get file input
	if(argc != 2){
		cout << ERROR << "Please include file to open" << endl;
		return -1;
	}
	
	getWords(argv[1]);
	// get doubly linked list of words
//	node* head;
//
//	cout << "Test 1: Words in order" << endl;
//
//	cout << "Test 2: Words in reverse order" << endl;
//
//	cout << "Test 3: Remove the 2nd item" << endl;
//
//	cout << "Test 4: Words in order" << endl;

	return 0;
}

