SHELL := /bin/bash
FLAGS = -Wall -Werror -Wextra
CC = gcc
OUT = ./s21_grep
FILE = grep_test.txt
PATTERNS = s21_greppat.reg

all:
	make s21_grep

s21_grep:
	$(CC) $(FLAGS) s21_grep.c -o s21_grep

cpp:
	cppcheck --enable=all --check-config --suppress=missingIncludeSystem s21_grep.c

clean:
	rm s21_grep

rebuild:
	make clean all

clang:
	clang-format -n *.c

tests:
	-diff <($(OUT) summer $(FILE)) <(s21_grep summer $(FILE))
	-diff <($(OUT) -v summer $(FILE)) <(s21_grep -v summer $(FILE))
	-diff <($(OUT) -i summer $(FILE)) <(s21_grep -i summer $(FILE))
	-diff <($(OUT) -iv summer $(FILE)) <(s21_grep -iv summer $(FILE))
	-diff <($(OUT) -in summer $(FILE)) <(s21_grep -in summer $(FILE))
	-diff <($(OUT) -ie "summer" $(FILE)) <(s21_grep -ie "summer" $(FILE))
	-diff <($(OUT) -ince "summer" $(FILE)) <(s21_grep -ince "summer" $(FILE))
	-diff <($(OUT) -lh ve $(FILE)) <(s21_grep -lh ve $(FILE))
	-diff <($(OUT) -hc e $(FILE)) <(s21_grep -hc e $(FILE))
	-diff <($(OUT) -oc summer $(FILE)) <(s21_grep -oc summer $(FILE))
	-diff <($(OUT) -s summer file.txt) <(s21_grep -s summer file.txt)
	-diff <($(OUT) -e summer -e summer $(FILE)) <(s21_grep -e summer -e summer $(FILE))
	-diff <($(OUT) -s summer $(FILE)) <(s21_grep -s summer $(FILE))
	-diff <($(OUT) -f $(PATTERNS) $(FILE)) <(s21_grep -f $(PATTERNS) $(FILE))
	-diff <($(OUT) -oi summer $(FILE)) <(s21_grep -oi summer $(FILE))
	-diff <($(OUT) -ohf $(PATTERNS) $(FILE)) <(s21_grep -ohf $(PATTERNS) $(FILE))
	-diff <($(OUT) -oinc summer $(FILE)) <(s21_grep -oinc summer $(FILE))
	-diff <($(OUT) -oihln -e summer -e ve -f $(PATTERNS) $(FILE)) <(s21_grep -oihln -e summer -e ve -f $(PATTERNS) $(FILE))
