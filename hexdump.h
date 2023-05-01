#include <stdio.h>
#include <string.h>

// Param: input (int) -> binary representation of the nibble to be converted
// Returns the char representation of the nibble
char to_hex(int input);

// Param: input (char) -> binary representation of the byte currently being worked on
// Param: position (int) -> position of the bit
// Returns the value of the bit at the specified position, in bool form
bool get_hbit(char input, int position);

// Param: input (char) ->
// Returns 'normalized ascii'. If the input is printable, it is simply returned. 
// If the input is not printable, but is a valid ASCII char, ~ is returned
// If the input is not a valid ASCII char, . is returned
char normalize_ascii(int input);

// Param: file (FILE pointer) -> the file to be read
// Outputs the hexdump to the terminal window
// Returns 0 if success, -1 if fail
int hexdump(FILE *file);

