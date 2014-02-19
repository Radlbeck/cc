/* This is a template file for the AES-128 key scheduling project. */
// Edited By: Andrew Radlbeck
/* include header files */
#include    <stdio.h>
#include    <stdlib.h>

/* This is how you can define a macro for constants */
#define	    NW	4       // Number of 32-bit words in a block
#define	    NB	(NW*4)  // Number of bytes in a block
#define	    NR	10  	// Number of rounds 

// Number of words in round keys
#define	    NW_ROUNDKEY	    (NW*(NR+1))

/* define new data types */
typedef	unsigned char  UINT8;
typedef	unsigned int   UINT32;

/** A function that prints a section of memory.
 *
 *  You can change the width (w), which decides how many bytes you want to put on each line.
 */
static void	print_char (unsigned char *p, int n)
{
	const	int	w = 16;
	int	i = 0;

	while (i < n) {
	    int	j;
	    printf("%04X(%5d):\t", i, i);
	    for (j = 0; j < w && i < n; j ++) {
		printf("%02X ", *p ++);
		i ++;
	    }
	    printf("\n");
	}
	puts("");
}

/* Another function to print out a word */
void	print_unsigned_int (unsigned int *p, int n)
{
	const	int	w = 4;
	int	i = 0;

	while (i < n) {
	    int	j;
	    printf("%04X(%5d):\t", i, i);
	    for (j = 0; j < w && i < n; j ++) {
		printf("%08X ", *p ++);
		i ++;
	    }
	    printf("\n");
	}
	puts("");
}

static UINT8  aes_SBOX [256] = {
0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
};

/* constants in key scheduling. 
 * Only the higher byte is stored here. Lower 24 bytes should be 0 */
static UINT8 RCon[] = {
0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a
};

/** key scheduling for AES-128 
    @param in mkey  Master key. 16 bytes for AES-128
    @param out roundkey Pointers to buffer that will store round key.

    You have to make sure you have allocated enough space for the round keys. 
 **/
void	aes128_encrypt_key_scheduling (UINT8 *mkey, UINT32 *rkey)
{
    int	    i;

    /* copy the master key to round key. 
     * Note the data type has changed from bytes to words.
     * AES assumes big-endian words */
    for (i = 0; i < NW; i ++) {
	int s = i * 4;
	rkey[i] = (mkey[s+0] << 24) | (mkey[s+1] << 16) | (mkey[s+2] << 8) | (mkey[s+3]); 
    }
    /* put your code here */
    UINT32 t = 0;
    for (i = i; i < (NW_ROUNDKEY); i++){				// loop threw all the words
	if(!(i % NW)){ 							// every 4 words generate t_i
		 t = rkey[i-1];
		// 8-bit rotated SBOX sub given {a0,a1,a2,a3}
		t = (aes_SBOX[((t >> 16) & 0x000000FF)] << 24) |	// a1
		    (aes_SBOX[((t >> 8) & 0x000000FF)]  << 16) | 	// a2
		    (aes_SBOX[(t & 0x000000FF)] << 8) 	       |	// a3
		    aes_SBOX[(t >> 24)];				// a0
		
		t = t ^ (RCon[(i/NW) - 1] << 24);			// XOR with the RCon array who stores only the high bits (<< 24)
		rkey[i] = t ^ rkey[i - NW];				// XOR with the first word in the last round
	}else{
		rkey[i] = rkey[i - 1] ^ rkey[i - NW];			// Everyother word, simply XOR with its previous round version
	}
    }
    return;
}

#define BUFFER_SIZE 100
// returns true and copys key given user input was received correctly
int get_user_key(UINT8 *mkey)
{
	printf("Please enter a key: ");
	
	UINT8 temp_mkey[NB];
	char in[BUFFER_SIZE];	// assuming the user doesn't go crazy with white space
	char twos[2];		// temporarily store 8-bit hex word
	int twos_count = 0;
	int key_count = 0;	
	fgets(in, BUFFER_SIZE, stdin);

	int i = 0;		
	for(i = 0; i < BUFFER_SIZE; i++){              // rotate threw input
		if(!isspace(in[i])){		       // ignore whitespace
			if(!isxdigit(in[i])) return 0; // valid hex? (also handles too few inputs)
			twos[twos_count] = in[i];
			if(twos_count >= 1){	       // once a pair has been found convert to UINT8
				temp_mkey[key_count] = (UINT8) strtol(twos, NULL, 16);
				if(key_count == (NB - 1)){ 	  // upon first valid 32 8-bit words exit
					for(i = 0; i < NB; i++)   // copy temp into the real key
						mkey[i] = temp_mkey[i];
				       	return 1;
				}
				key_count++;
				twos_count = 0;
			}else{
				twos_count++;
			}
		}
	}
	
	return 0;
}

/* Define buffers for the keys.
 * The master key is an array of bytes.
 * The round_keys can be an array of bytes or an array of words (32-bit). You can change it.   
 * */
static UINT8	maskter_key[NB] = {
0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
};
static UINT32	round_keys[NW_ROUNDKEY];

int main (int argc, char **argv) 
{
    if(!get_user_key(maskter_key)){	 // printf("Error reading entry.... using default key\n"); Could continue if desired 
	printf("Error: Not a valid input\n");
        return -1;
    }
  
    printf("The master key is:\n");
    print_char(maskter_key, NB);

    aes128_encrypt_key_scheduling(maskter_key, round_keys);

    printf("The round_keys are:\n");
    print_unsigned_int(round_keys, NW_ROUNDKEY);
    return 0;
}

