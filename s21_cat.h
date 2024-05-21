#ifndef S21_CAT_H_
#define S21_CAT_H_

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>


#define _GNU_SOURCE

typedef struct {
    int b, e, E, v, n, s, t, T;
}Flags;

Flags parser(int argc, char** argv);
char flag_v(unsigned char ch);
void flags_E_T_v(Flags* flag, char* line_text, int n);
void output_file_flags(Flags* flag, const char* argv);
void output_file(FILE* fp);


#endif  // S21_CAT_H_
