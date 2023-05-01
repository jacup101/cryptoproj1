#include <stdio.h>
#include <string.h>


char to_base64(int input);

// Takes in a string to a filepath
// Returns 0 if success, -1 if fail
int encode_to_64(FILE *file);

// Takes in a string to a filepath of encoded base64 code
int decode_to_64(FILE *file);


