#include "s21_sprintf.h"

#include <math.h>
struct flags {
  int minus;
  int plus;
  int space;
  int hash_sign;
  int zero;
  int width;
  int accuracy;
  int length;
  int was_acc;
} flags = {0};

void flag_reset() {
  flags.minus = 0;
  flags.plus = 0;
  flags.space = 0;
  flags.hash_sign = 0;
  flags.zero = 0;
  flags.width = 0;
  flags.accuracy = 0;
  flags.length = 0;
  flags.was_acc = 0;
}

int s21_sprintf(char *str, const char *format, ...) {
  va_list arguments;
  va_start(arguments, format);

  int i = 0;
  while (*format) {
    if (*format == '%') {
      format++;
      kate_kek_parser(&format);
      switch (*format) {
        case 'c':
          handle_for_c(str, &i, va_arg(arguments, int));
          flag_reset();
          break;
        case 'd':
          handle_for_d_u(str, &i, va_arg(arguments, int));
          flag_reset();
          break;
        case 'f':
          handle_for_f(str, &i, va_arg(arguments, double));
          break;
        case 's':
          handle_for_s(str, &i, va_arg(arguments, char *));
          flag_reset();
          break;
        case 'u':
          handle_for_d_u(str, &i, va_arg(arguments, int));
          flag_reset();
          //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!UUUUU!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
          break;
        default:
          // Handle unknown specifier
          break;
      }
    } else {
      str[i++] = *format;
    }
    format++;
  }

  str[i] = '\0';  // Null-terminate the string
  return i;
}

void kate_kek_parser(const char **format) {
  char *specificator_exist = "cdfsu";
  int width = 0;

  while (s21_strchr(specificator_exist, **format) == s21_NULL) {
    if (**format == '-') {
      flags.minus = 1;
      flags.zero = 0;  // Reset zero flag if minus flag is set
    } else if (**format == '+') {
      flags.plus = 1;
      flags.space = 0;  // Reset space flag if plus flag is set
    } else if (**format == ' ') {
      flags.space = 1;
    } else if (**format == '#') {
      flags.hash_sign = 1;
    } else if (**format == '0') {
      flags.zero = 1;
    } else if (**format >= 48 && **format <= 57 && !width) {
      flags.width = string_to_int(format);
      width = 1;
      (*format)--;
    } else if (**format == '.') {
      (*format)++;
      flags.accuracy = string_to_int(format);
      flags.was_acc = 1;
      (*format)--;
    } else if (**format == 'h' || **format == 'l' || **format == 'L') {
      flags.length = **format;
    } else {
      break;  // Unknown flag, stop parsing
    }

    (*format)++;
  }
}

int string_to_int(const char **format) {
  char *number = s21_NULL;
  int index = 0;

  while (**format >= 48 && **format <= 57) {
    number = realloc(number, sizeof(char) * (index + 1));
    number[index++] = **format;
    (*format)++;
  }
  number[index] = '\0';

  int result = 0;
  int degree = 1;

  for (index--; index >= 0; index--) {
    result += (number[index] - 48) * degree;
    degree *= 10;
  }

  free(number);
  return result;
}

void handle_for_c(char *str, int *position, char symbol) {
  flags.width--;
  if (!flags.minus) fill_width(str, position);
  str[*position] = symbol;
  (*position)++;
  if (flags.minus) fill_width(str, position);
}

void fill_width(char *str, int *position) {
  char fill;
  if (flags.zero && !flags.minus)
    fill = '0';
  else
    fill = ' ';

  for (int i = 0; i < flags.width; i++) {
    str[*position] = fill;
    (*position)++;
  }
}

int int_to_string(char *buffer, int value) {
  int i = 0;

  if (value < 0) value = -value;

  if (value == 0) {
    buffer[i++] = '0';
  }

  while (value != 0) {
    buffer[i++] = '0' + value % 10;
    value /= 10;
  }

  for (int j = 0; j < i / 2; j++) {
    char tmp = buffer[j];
    buffer[j] = buffer[i - j - 1];
    buffer[i - j - 1] = tmp;
  }

  buffer[i] = '\0';
  return i;
}

void handle_for_s(char *str, int *position, char *input) {
  s21_size_t input_length = s21_strlen(input);

  if ((flags.accuracy == 0 && flags.was_acc) || flags.accuracy != 0)
    input_length = flags.accuracy;
  if ((flags.width - flags.accuracy) >= 0)
    flags.width -= input_length;
  else
    flags.width = 0;

  if (flags.minus) {
    for (s21_size_t i = 0; i < input_length; i++) {
      str[*position] = input[i];
      (*position)++;
    }
    fill_width(str, position);
  } else {
    fill_width(str, position);
    for (s21_size_t i = 0; i < input_length; i++) {
      str[*position] = input[i];
      (*position)++;
    }
  }
}

void handle_for_d_u(char *str, int *position, int value) {
  char buffer[256];
  int length = int_to_string(buffer, value);
  // s21_size_t str_length = s21_strlen(str);

  if (flags.minus) {
    if (flags.plus || value < 0) {
      str[*position] = choose_sign(value);
      (*position)++;
    }

    for (int i = 0; i < flags.accuracy - length; i++) {
      str[*position] = '0';
      (*position)++;
    }
    *position += flags.accuracy - length >= 0 ? flags.accuracy - length : 0;

    for (int i = 0; i < length; i++) {
      str[*position] = buffer[i];
      (*position)++;
    }

    flags.width -= flags.accuracy > length ? flags.accuracy : length;
    fill_width(str, position);
  } else {
    // if (flags.was_acc) flags.zero = 0;
    flags.width -= flags.accuracy > length ? flags.accuracy : length;
    flags.width -= flags.plus || value < 0 ? 1 : 0;
    fill_width(str, position);

    if (flags.plus || value < 0) {
      str[*position] = choose_sign(value);
      (*position)++;
    }

    for (int i = 0; i < flags.accuracy - length; i++) {
      str[*position] = '0';
      (*position)++;
    }

    for (int i = 0; i < length; i++) {
      str[*position] = buffer[i];
      (*position)++;
    }
  }
}

void handle_for_f(char *str, int *position, double value) {
  char buffer[256];
  int int_part = (int)value;
  int int_length = int_to_string(buffer, int_part);

  int precision = flags.accuracy == 0 && !flags.was_acc ? 6 : flags.accuracy;

  flags.width -= precision;
  flags.width--;
  flags.width -= int_length;
  if (flags.minus) {
    // // Обработка знака
    if (flags.plus || value < 0) {
      str[*position] = choose_sign(value);
      (*position)++;
    }
    if (flags.plus || value < 0) flags.width--;

    for (int i = 0; i < int_length; i++) {
      str[*position] = buffer[i];
      (*position)++;
    }

    // // Обработка целой част

    // Обработка дробной части
    if (precision > 0 || flags.hash_sign) {
      str[*position] = '.';
      (*position)++;
      double frac_part = value - (int)value;
      if (frac_part < 0) frac_part = -frac_part;
      for (int i = 0; i < precision; i++) {
        //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!ОКРУГЛЕНИЕ!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        frac_part *= 10;
        int digit = (int)frac_part;
        str[*position] = '0' + digit;
        (*position)++;
        frac_part -= (int)frac_part;
      }
    }

    fill_width(str, position);

  } else {
    if (flags.plus || value < 0) flags.width--;
    fill_width(str, position);

    if (flags.plus || value < 0) {
      str[*position] = choose_sign(value);
      (*position)++;
    }

    for (int i = 0; i < int_length; i++) {
      str[*position] = buffer[i];
      (*position)++;
    }

    // // Обработка целой част

    // Обработка дробной части
    if (precision > 0 || flags.hash_sign) {
      str[*position] = '.';
      (*position)++;
      double frac_part = value - (int)value;
      if (frac_part < 0) frac_part = -frac_part;
      for (int i = 0; i < precision; i++) {
        //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!ОКРУГЛЕНИЕ!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        frac_part *= 10;
        int digit = (int)frac_part;
        str[*position] = '0' + digit;
        (*position)++;
        frac_part -= (int)frac_part;
      }
    }
  }
}

char choose_sign(int value) {
  char sign;
  if (value >= 0)
    sign = '+';
  else
    sign = '-';
  flags.width--;
  return sign;
}
