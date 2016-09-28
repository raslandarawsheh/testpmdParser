#this is a simple MakeFile example
CC = gcc
CFLAGS =  -lc -g
RM = rm -f

default: all

all: parser
parser: parser.c
	$(CC) $(CFLAGS) -o parser parser.c parser.h
clean veryclean:
	$(RM) parser
