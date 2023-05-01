#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "base64.h"

char to_base64(int input) {
    if (input >= 0 && input <= 25) {
        return 'A' + input;
    }
    if (input >= 26 && input <= 51) {
        return 'a' + (input - 26);
    }
    if (input >= 52 && input <= 61) {
        return '0' + (input - 52);
    }
    if (input == 62) {
        return '+';
    }
    if (input == 63) {
        return '/';
    }
    return '=';
}

int to_base2(char input) {
    if (input >= 'A' && input <= 'Z') {
        return 0 + (input - 'A');
    }
    if (input >= 'a' && input <= 'z') {
        return 26 + (input - 'a');
    }
    if (input >= '0' && input <= '9') {
        return 52 + (input - '0');
    }
    if (input == '+') {
        return 62;
    }
    if (input == '/') {
        return 63;
    }
    if (input == '=') {
        // Here we can return anything except -1, the actual handling for == is handled elsewhere. 
        // We default to -2
        return -2;
    }
    printf("Improper character %c for base 64, please ignore any prior output as this can not be decoded...\n", (char) input);
    return -1;
}

bool get_bit(char input, int position) {
    return  (input >> position) & 0x1;
}

int encode_to_64(FILE *file) {
    // A bunch of inputs we need to consider
    int buffer_size = 12;
    int total_output = 0;
    char buffer[buffer_size];
    int read = 0;
    int curr_64_byte_sum = 0;
    int line_size = 64;

    read = fread(buffer, sizeof(char), buffer_size, file);
    // printf("Read %d bytes\n", read);

    for (int curr_bit = 0; ; curr_bit ++ ) {
        if (curr_bit >= (buffer_size * 8)) {
            curr_bit = 0;
            read = fread(buffer, sizeof(char), buffer_size, file);
            if (read == 0 ) {
                // End of file reached nicely, break out;
                break;
            }
        }
        // Temporary, eventually make it pad rather than break
        if ((curr_bit / 8) + 1 > read) {
            if (read % 3 == 1) {
                printf("%c", to_base64(curr_64_byte_sum));
                printf("==");
            }
            if (read % 3 == 2) {
                printf("%c", to_base64(curr_64_byte_sum));
                printf("=");
            }
            printf("\n");
            break;
        }

        int current_byte = curr_bit / 8;
        char curr_char = buffer[current_byte];
        bool bit_val = get_bit(curr_char, 7 - (curr_bit % 8));

        // Reset 
        if (curr_bit % 6 == 0) {
            curr_64_byte_sum = 0;
        }

        if (bit_val) {
            curr_64_byte_sum += pow(2, 5 - (curr_bit % 6));
        }

        // End
        if (curr_bit % 6 == 5) {
            printf("%c", to_base64(curr_64_byte_sum));
            total_output ++;
            if (total_output % line_size == 0) {
                printf("\n");
            }
        }
    }
    return 0;
}

int decode_to_64(FILE *file) {
    // A bunch of inputs we need to consider
    int buffer_size = 4;
    char buffer[buffer_size];
    int read = 0;

    read = fread(buffer, sizeof(char), buffer_size, file);
    // printf("Read %d bytes\n", read);
    int curr_byte_sum = 0;
    int curr_bit_num = 0;

    for (int curr_bit = 0; ; curr_bit ++ ) {
        int current_byte = curr_bit / 8;
        if (buffer[current_byte] == '\n') {
            curr_bit += 8;
            current_byte = curr_bit / 8;
        }
        if (curr_bit % 8 == 0) {
            curr_bit += 2;
            current_byte = curr_bit / 8;
        }
        if (curr_bit % 8 == 1) {
            curr_bit += 1;
            current_byte = curr_bit / 8;
        }
        
        

        if (curr_bit >= (buffer_size * 8)) {
            curr_bit = 0;
            read = fread(buffer, sizeof(char), buffer_size, file);
            if (read == 0 ) {
                // End of file reached nicely, break out;
                break;
            }
        }
        current_byte = curr_bit / 8;
        // skip bits
        if (buffer[current_byte] == '\n') {
            curr_bit += 8;
            current_byte = curr_bit / 8;
        }
        if (curr_bit % 8 == 0) {
            curr_bit += 2;
            current_byte = curr_bit / 8;
        }
        if (curr_bit % 8 == 1) {
            curr_bit += 1;
            current_byte = curr_bit / 8;
        }
        
        // Temporary, eventually make it pad rather than break
        if ((curr_bit / 8) + 1 > read) {
            break;
        }

        char curr_char = to_base2(buffer[current_byte]);
        if (curr_char == -1) {
            return -1;
        }
        // printf("%c%d ", buffer[current_byte], curr_bit);
        bool bit_val = get_bit(curr_char, 7 - (curr_bit % 8));

        if (buffer[current_byte] == '=') {
            break;
        }
        // Reset 
        if (curr_bit_num == 0) {
            // Reset everything
            curr_byte_sum = 0;
        }

        if (bit_val) {
            curr_byte_sum += pow(2, 7 - curr_bit_num);
            // printf("1 ");
        } else {
            // printf("0 ");
        }

        // End
        if (curr_bit_num == 7) {
            printf("%c", (char) curr_byte_sum);
            curr_bit_num = 0;
        } else {
            curr_bit_num += 1;
        }
        // Always increment the bits
    }
    return 0;
}