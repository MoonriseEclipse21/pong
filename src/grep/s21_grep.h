#ifndef GREF_H
#define GREF_H
#define _GNU_SOURCE

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct options {
  int regex, invert, count, files, line_num, no_file_name, error_null,
      match_part, file_pattern;
} flags = {0};

struct patterns {
  char **patterns;
  int count_patterns;
} patterns = {NULL, 0};

struct files {
  char **files;
  int count_files;
} files = {NULL, 0};

void parser(int arg_count, char **arg_array, int *error_code);
void files_parser(int arg_count, char **arg_array, int *error_code);
void add_pattern(char flag, char *optarg, int *error_code);

void grep_output();
void grep_utility(FILE *file, char *filename, struct options flags,
                  struct patterns patterns);
void grep_count(int match_count, char *filename, struct options flags);
void grep_files_print(char *filename, struct options flags);
void grep_print(char *line, char *filename, int count, struct options flags);
void grep_match_part(char *line, char *filename, int count, regex_t *preg);

void free_all();
#endif
