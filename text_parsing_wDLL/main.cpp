// Code for basic data manipulation
//
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <algorithm>
using namespace std;

#define ERROR        "Error: "
#define MAX_WORD_LEN 100

struct node{				// node data structure for DLL
	char word[MAX_WORD_LEN];
	node * next;
	node * prev;
};

void printDLL_forward(node * head)
{
	node * current = head;
	while(current->next != NULL ){		// rotate threw list untill end
		cout << current->word << ", ";  // print out the current word
		current = current->next;	// set the next node to be the current
	}
	cout << current->word << endl;
}

node * getWords(char * infile)
{
	node * head = NULL; 
	node * prev = NULL;

	ifstream file (infile);								// open file into an input file stream
	if(file.is_open()){
		string word;
		while(file >> word){							// stream operator parses on white space (seperating words)
			cout << "on word: " << word << endl;//TODO REMOVE
			transform(word.begin(), word.end(), word.begin(), ::tolower);	// make sure all words are lower case

			node * current = new node;		// create a new node pointer & allocate memory
			node * find = head;
			char dbreak = 0;			// flag to help skip word

			strcpy(current->word, word.c_str());	// copy the lower case word into the node

			while(find != NULL){
				if((string)find->word < (string)current->word){
					cout << "\t" << find->word << " < " << current->word << endl; //TODO REMOVE
					prev = find;
					find = find->next;
				}else if((string)find->word == (string)current->word){
					dbreak = 1;
					delete current;
					break;
				}else{
					break;
				}
			}

			if(dbreak) continue;
			if(prev != NULL){
			       	prev->next    = current;		// set next, prev pointers
				current->prev = prev;

				current->next = find;
				find->prev    = current;
				cout << "\t\t\t" << "link" << endl;	//TODO REMOVE
			}else{
				if(head == NULL){
				       	head = current;
					cout << "\t\t\t" << "head" << endl;	//TODO REMOVE
				}else{
					current->next = head;
					head->prev = current;
					head = current;



					cout << "\t\t\t" << "p head" << endl; 	//TODO REMOVE
				}
			}
			printDLL_forward(head);	//TODO REMOVE

		}
		file.close();
	}else{
		cout << ERROR << "cannot open file" << endl;
		return NULL;
	}
	return head;
}


void printDLL_backward(node * head)
{
	node * current = head;
	while(current->next != NULL){		// find the tail
		current = current->next;
	}

	while(current->prev != NULL){		// reversed print forward
		cout << current->word << ", ";
		current = current->prev;
	}
	cout << current->word << endl;
}

void remove2ndNode(node * head)
{
	node * current = head;

	current = current->next;
	current->next->prev = current->prev;	// create the link around the deleted node
	current->prev->next = current->next;
	delete current;				// deallocate memory for node
}

int main(int argc, char * argv[])
{
	// get file input
	if(argc != 2){
		cout << ERROR << "Please include file to open" << endl;
		return -1;
	}
	node * head = getWords(argv[1]);
	
	cout << "Test 1: Words in order" << endl;
	printDLL_forward(head);

	cout << endl << "Test 2: Words in reverse order" << endl;
	printDLL_backward(head);
	
/*	cout << endl << "Test 3: Remove the 2nd item" << endl;
	remove2ndNode(head);

	cout <<"Test 4: Words in order" << endl;
	printDLL_forward(head);
*/
	return 0;
}

