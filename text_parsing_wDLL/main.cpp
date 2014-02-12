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
			//TODO sort the words (as chars), in order, into a dll
		}
		file.close();
	}else{
		cout << ERROR << "cannot open file" << endl;
		return 0;
	}
	return -1;
}

void printDLL_forward(node * head)
{
	node * current = head;
	while(current != NULL ){		// rotate threw list untill end
		cout << current->word << ", ";  // print out the current word
		current = current->next;	// set the next node to be the current
	}
	cout << endl;
}

void printDLL_backward(node * head)
{
	node * current = head;
	while(current->next != NULL){		// find the tail
		current = current->next;
	}

	while(current != NULL){
		cout << current->word << ", ";
		current = current->prev;
	}
	cout << endl;
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

