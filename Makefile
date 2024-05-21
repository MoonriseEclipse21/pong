SHELL:= /bin/bash
FLAGS = -Wall -Werror -Wextra
CC = gcc
OUT = ./s21_cat
FILE = cat_test.txt

all:
 make s21_cat

s21_cat:
 $(CC) $(FLAGS) s21_cat.c -o s21_cat

cpp:
 cppcheck --enable=all --check-config --suppress=missingIncludeSystem s21_cat.c

clean:
 rm s21_cat

clang:
 clang-format -n *.c

tests:

 -diff <($(OUT) -b $(FILE)) <(cat -b $(FILE))
 -diff <($(OUT) -e $(FILE)) <(cat -e $(FILE))
 -diff <($(OUT) -n $(FILE)) <(cat -n $(FILE))
 -diff <($(OUT) -s $(FILE)) <(cat -s $(FILE))
 -diff <($(OUT) -t $(FILE)) <(cat -t $(FILE))
