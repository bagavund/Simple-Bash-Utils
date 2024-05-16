CC=gcc
CFLAGS=-Wall -Werror -Wextra
SRCS=s21_cat.c
LIB=s21_cat.h

.PHONY: 
	all s21_cat test clean

all:  clean s21_cat

s21_cat:
	$(CC) $(CFLAGS) $(SRCS) $(LIB) -o s21_cat

test: all
	bash s21_cat_test.sh

clean: 
	rm -rf *.o *.out log.txt s21_cat
