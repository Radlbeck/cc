/*
 *  Lab 0 - Basic data and file manipulation
 *
 *	By: Andrew Radlbeck & John Wood
 */

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
	cout << current->word << endl;		// print last node
}

node * getWords(char * infile)
{
	node * head = NULL; 
	node * prev = NULL;

	ifstream file (infile);								// open file into an input file stream
	if(file.is_open()){
		string word;
		while(file >> word){							// stream operator parses on white space (seperating words)
			transform(word.begin(), word.end(), word.begin(), ::tolower);	// make sure all words are lower case

			node * current = new node;		// create a new node pointer & allocate memory
			node * find = head;
			char dbreak = 0;			// flag to help skip word

			strcpy(current->word, word.c_str());	// copy the lower case word into the node

			while(find != NULL){						// loop threw the current DLL
				if((string)find->word < (string)current->word){		// search untill word finds a higher word (alpebetically)
					prev = find;					// increment pointer
					find = find->next;
				}else if((string)find->word == (string)current->word){	// ignore repeat words
					dbreak = 1;
					delete current;					// deallocate memory
					break;
				}else{
					break;						// once location is found leave the loop
				}
			}

			if(dbreak) continue;						// if duplicate skip current word
			if(prev != NULL){
			       	prev->next    = current;		// set next, prev pointers to fit diagram below
				current->prev = prev;

				current->next = find;			// [prev] <-> [current] <-> [find]
				find->prev    = current;
			}else{
				if(head == NULL){			// if list is empty create head
				       	head = current;
				}else{
					current->next = head;		// if the word replaces the head adjust pointers
					head->prev = current;
					head = current;			// [current] <-> [head]
				}
			}
		}
		file.close();					// close file
	}else{
		cout << ERROR << "cannot open file" << endl;	// if file open fails print an error
		return NULL;			
	}
	return head;
}


void printDLL_backward(node * head)
{
	node * current = head;
	while(current->next != NULL){		// find the tail
		current = current->next;	// increment position pointer
	}

	while(current->prev != NULL){		// reversed printDLL_forward
		cout << current->word << ", ";
		current = current->prev;	// decrement position pointer
	}
	cout << current->word << endl;		// print first word
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
	if(argc != 2){				// Check for corrent number of parameters
		cout << ERROR << "Please include file to open" << endl;
		return -1;			// return with an error condition
	}
	node * head = getWords(argv[1]);	// get DLL from text file
	
	cout << "Test 1: Words in order" << endl;
	printDLL_forward(head);

	cout << endl << "Test 2: Words in reverse order" << endl;
	printDLL_backward(head);
	
	cout << endl << "Test 3: Remove the 2nd item" << endl;
	remove2ndNode(head);

	cout <<"Test 4: Words in order" << endl;
	printDLL_forward(head);

	return 0;
}

