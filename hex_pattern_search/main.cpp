// Lab 2 - Memory Remenance
// Code for basic file search
#include <cstdio>
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
	// get input file
	if(argc != 2){	
		cout << "No input file specified" << endl;
		return 1;
	}

	// print/search file contents
	FILE * pfile;
	int current_bit = 0;
	int pattern_aa  = 0xaa;
	int pattern_55  = 0x55;
	int count_aa = 0; 	// ª
	int count_55 = 0;	// U
	int count_error = 0;
	int count_word  = 0;
	float percent_error = 0;
	bool was_aa = 1;	// starts with 0x55 'U'
	bool pattern_not_found = 1;

	pfile = fopen(argv[1], "r");

	if(pfile != NULL){
		while(current_bit != EOF){
			current_bit = fgetc(pfile);		// get next word from file
			if(current_bit == EOF) break;		// end?	
			count_word++;

			if(current_bit == pattern_aa && !was_aa){		
				count_aa++;
				was_aa = 1;
			}else if(current_bit == pattern_55 && was_aa){
				count_55++;
				was_aa = 0;
			}else{
				int error_bit   = 0;
				if(was_aa)					// should be 0x55
					error_bit = 0x55 ^ current_bit;
				else						// should be 0xaa
					error_bit = 0xaa ^ current_bit;
				count_error += __builtin_popcount(error_bit);   // count the set bits
				was_aa = !was_aa;				// toggle last pattern
			}

			// assuming that the percent error will not excede 40%
			// within the first 25 words of the pattern
			if(!(count_word % 25) && pattern_not_found){ 	// Check percentage every 25 words
				percent_error =  ((count_error)*100) / (count_word * 8);
				if(percent_error > 40){
					count_word  = 0;
					count_error = 0;
					count_aa = 0;
					count_55 = 0;
				}else{
					pattern_not_found = 0;
					cout << "Found it!!" << endl << endl;
				}
			}
			if(count_word >= 524288) break;	// 4,194,304 bytes / 8
		}
		fclose(pfile);
	}else{
		cout << "File cannot be opened" << endl;
		return 1;
	}
	
	// display metrics
	percent_error =  ((count_error)*100) / (count_word * 8);
	cout << "count 0x55: "  << count_55    << endl;
	cout << "count 0xaa: "  << count_aa    << endl;
	cout << "error bit count(bits fliped): " << count_error << endl;
	cout << "error percentage: " << percent_error << " %" << endl;

	return 0;
}

