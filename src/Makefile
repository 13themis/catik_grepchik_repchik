CC=gcc
CFLAGS=-Wall -Wextra -Werror -std=c11

all: s21_cat s21_grep

s21_cat: ./cat/s21_cat.c
	$(CC) $(CFLAGS) ./cat/s21_cat.c -o ./cat/s21_cat

s21_grep: ./grep/s21_grep.c
	$(CC) $(CFLAGS) ./grep/s21_grep.c -o ./grep/s21_grep

clean:
	rm -rf ./cat/s21_cat
	rm -rf ./grep/s21_grep