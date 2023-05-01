#
#  Project 1 of COMP389: Cryptography
#  Function: Makefile
#  File Name: Makefile
#

PROJ = proj1

CC = gcc
CFLAGS = -g -Wall
RM = rm -f
TAR = gtar czvf proj1.tar.gz
SOURCE = proj1.c base64.c base64.h hexdump.c hexdump.h
SUPPORT = Makefile README
OBJS=proj1.o base64.o hexdump.o

default: all

all: $(PROJ)


proj1.o: proj1.c
	$(CC) $(CFLAGS) -c proj1.c

base64.o: base64.c base64.h
	$(CC) $(CFLAGS) -c base64.c

hexdump.o: hexdump.c hexdump.h
	$(CC) $(CFLAGS) -c hexdump.c

proj1: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o proj1


clean: 
	$(RM) *.o proj1 *~

tar: 
	$(TAR) $(SOURCE) $(SUPPORT)

