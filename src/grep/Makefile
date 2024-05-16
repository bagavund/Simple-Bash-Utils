CC = gcc
CFLAGS = -Wall -Werror -Wextra
SOURCES = s21_grep.c

s21_grep:
	$(CC) $(CFLAGS) $(SOURCES) -o s21_grep

test: s21_grep
	python3 test.py

clean:
	rm -rf s21_grep RESULT_VALGRIND.txt log.txt
