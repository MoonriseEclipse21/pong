CC = gcc
FLAGS = -Wall -Werror -Wextra -std=c11

all: s21_grep

s21_grep: s21_grep.o 
	$(CC) $(FLAGS) s21_grep.o -o s21_grep
	rm -rf *.o

s21_grep.o: s21_grep.c
	$(CC) $(FLAGS) -c s21_grep.c