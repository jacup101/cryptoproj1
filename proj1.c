/*
 * Project 1 of COMP389: Cryptography
 * Function: commandline parsing and file reading
 * File Name: proj1.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "base64.h"
#include "hexdump.h"

void usage(){
  printf("The commandline syntax for proj 1 is as follows:\n");
  printf("    proj1 {hexdump|enc-base64|dec-base64} [file]\n");
  exit(1);
}

int main(int argc, char *argv[]){
  if (argc < 2) {
    usage();
    return -1;
  }
  FILE *my_file;
  if (argc < 3) {
    my_file = stdin;
    fseek (stdin, 0, SEEK_END);
    int num = ftell (stdin);
    if (num == 0) {
      printf("Input of size 0 received, exiting...\n");
      return -1;
    }
  } else {
    my_file = fopen(argv[2], "rb");
  }
  if (my_file == NULL) {
    printf("Could not access file at %s\n", argv[2]);
    return -1;
  }
  


  if (strcmp(argv[1], "enc-base64") == 0) {
    // Run encoding
    return encode_to_64(my_file);
  }
  else if (strcmp(argv[1], "dec-base64") == 0) {
    // Run encoding
    return decode_to_64(my_file);
  } else if (strcmp(argv[1], "hexdump") ==0 ) {
    return hexdump(my_file);
  } else {
    printf("Failed to provide proper usage\n");
    return -1;
  }


  // Should not have reached here
  printf("Bad input resulted in no output, this is an erro\n");
  return -1;
}
