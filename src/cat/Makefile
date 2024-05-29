CC = gcc
FLAGS = -Wall -Werror -Wextra -std=c11

all: s21_cat 

s21_cat: s21_cat.o 
	$(CC) $(FLAGS) s21_cat.o -o s21_cat

s21_cat.o: s21_cat.c
	$(CC) $(FLAGS) -c s21_cat.c

clean:
	rm -rf *.o