#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "hexdump.h"

char to_hex(int input) {
    if (input >= 0 && input <= 9) {
        return '0' + input;
    }
    if (input >= 10 && input <= 15) {
        return 'a' + (input - 10);
    }
    return '0';

}

bool get_hbit(char input, int position) {
    return  (input >> position) & 0x1;
}

char normalize_ascii(int input) {
    if (input >= ' ' && input <= '~') {
        // Printable
        return (char) input;
      } else if (input < 0) {
        // Invalid character
        return '~';
      } else {
        // Unprintable character
        return '.';
      }
      // This should not be reachable, just there to handle the unlikely event
      return ' ';
}

int hexdump(FILE *file) {
    // Properties we need to consider
    // Most importantly, buffer_size and read are in terms of bytes
    // halfway, total_output, and line_size are not in terms of bytes, but rather in terms of nibbles
    // buffer used to access data, ascii_chars used for the printing at the end
    int buffer_size = 16;
    int halfway = 16;
    int total_output = 0;
    char buffer[buffer_size];
    char ascii_chars[buffer_size + 1];
    ascii_chars[buffer_size] = '\0';
    int read = 0;
    int curr_16_byte_sum = 0;
    int line_size = 32;

    read = fread(buffer, sizeof(char), buffer_size, file);
    // printf("Read %d bytes\n", read);
    // Print the first ID line
    printf("%.6x: ", total_output);

    // Loop through the bits
    for (int curr_bit = 0; ; curr_bit ++ ) {
        // If we have read through our buffer, attempt to read more data
        if (curr_bit >= (buffer_size * 8)) {
            // Reset curr_bit
            curr_bit = 0;
            // Read from the buffer
            read = fread(buffer, sizeof(char), buffer_size, file);
            if (read == 0 ) {
                // End of file reached nicely, break out
                break;
            }
            // If file has not ended, then print another ID line
            // Note that this only works because we are reading in 16 bytes at a time
            if (read != 0) {
                printf("%.6x: ", total_output / 2);
            }
        }
        // Get the value of the current bit
        int current_byte = curr_bit / 8;
        char curr_char = buffer[current_byte];
        bool bit_val = get_hbit(curr_char, 7 - (curr_bit % 8));
        
        // Reset when accessing a new nibble
        if (curr_bit % 4 == 0) {
            curr_16_byte_sum = 0;
        }
        // At each bit, at the value to the byte sum
        if (bit_val) {
            curr_16_byte_sum += pow(2, 3 - (curr_bit % 4));
        }
        // At the end of an entire bit, add the actual ascii character to the array
        if (curr_bit % 8 == 7) {
            if (current_byte >= read) {
                ascii_chars[current_byte] = ' ';
            } else {
                ascii_chars[current_byte] = normalize_ascii(buffer[current_byte]);
            }
        }
        // At the end of the nibble, print out the hex value
        if (curr_bit % 4 == 3) {
            // If we actually have nothing to read there, print a -
            // Without this, it would just print data from the last read
            if (current_byte >= read) {
                printf("-");
            } 
            // Print the hex value otherwise
            else {
                printf("%c", to_hex(curr_16_byte_sum));
            }
            // Print spaces whenever appropriate
            total_output ++;
            if (total_output % 2 == 0) {
                printf(" ");
            }
            // Add an extra space at the halfway mark
            if (total_output % halfway == 0) {
                printf(" ");
            }
            // Print a new line when we reach the end of the 16 bytes
            if (total_output % line_size == 0) {
                printf("%s\n", ascii_chars);
            }
        }
    }
    return 0;
}
