#ifndef S21_GREP_H_
#define S21_GREP_H_

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _GNU_SOURCE

typedef struct flags {
  int e, i, v, c, l, n, h, s, f, o;
  char* pattern;
  int lenght_of_pattern;
  int memory_for_pattern;
} flags;

flags parser(int argc, char** argv);
void add_pattern(flags* flag, char* pattern);
void flag_f(flags* flag, char* file_path);
void reg_comp(flags flag, int argc, char** argv);
void processing_file_pattens(flags flag, char* path, regex_t* reg);
void output_text(char* file_text, int n);
void flag_o(regex_t* reg, char* line_text);

#endif  // S21_GREP_H_
