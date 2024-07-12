#ifndef S21_SPRINTF_H
#define S21_SPRINTF_H

#include <stdarg.h>

#include "../s21_string/s21_string.h"

int s21_sprintf(char *str, const char *format, ...);
void kate_kek_parser(const char **format);
int string_to_int(const char **format);
void fill_width(char *str, int *position);
char choose_sign(int value);

void handle_for_c(char *str, int *position, char symbol);
void handle_for_d_u(char *str, int *position, int value);
void handle_for_s(char *str, int *position, char *input);
void handle_for_f(char *str, int *position, double value);

int int_to_string(char *buffer, int value);

#endif
