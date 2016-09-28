CC = gcc 
CFLAGS = -lm -lc -g
RM = rm -f

default: all

all: parser

parser: : pars.c 
    $(CC) $(CFLAGS) -o parser parser.c parser.h

clean veryclean:
    $(RM) parser
