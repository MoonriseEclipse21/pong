#include "s21_cat.h"

int main(int argc, char **argv) {
  int Error_code = 0;
  flag_parser(&options, argc, argv, &Error_code);
  if (!Error_code)
    print_file(argc, argv, &options);
  else
    print_error();
}

void flag_parser(struct options *options, int argc, char **argv,
                 int *Error_code) {
  for (int i = 1; i < argc; i++) {
    if ((argv[i][0] != '-' || !strcmp(argv[i], "--")) && (*Error_code == 0)) {
      break;
    } else {
      add_flag(options, argv[i], Error_code);
    }
  }
}

void add_flag(struct options *options, char *string_option, int *Error_code) {
  if (strncmp(string_option, "--", 2) == 0) {
    add_flag_long(options, string_option, Error_code);
    return;
  }

  for (size_t i = 1; i < strlen(string_option); i++) {
    switch (string_option[i]) {
      case 'b':
        options->b = 1;
        break;
      case 't':
        options->t = 1;
        options->v = 1;
        break;
      case 'T':
        options->t = 1;
        break;
      case 's':
        options->s = 1;
        break;
      case 'e':
        options->e = 1;
        options->v = 1;
        break;
      case 'n':
        options->n = 1;
        break;
      case 'v':
        options->v = 1;
        break;
      case 'E':
        options->e = 1;
        break;
      default:
        *Error_code = 1;
        break;
    }
  }
}

void add_flag_long(struct options *options, char *string, int *Error) {
  char *exist_long[3] = {"--number-nonblank", "--number", "--squeeze-blank"};
  int status = 2;

  for (int i = 0; i < 3; i++) {
    if (!strcmp(exist_long[i], string)) {
      status = 0;
      switch (i) {
        case 0:
          options->b = 1;
          break;
        case 1:
          options->n = 1;
          break;
        case 2:
          options->s = 1;
          break;
        default:
          break;
      }
    }
  }
  *Error = status;
}

void cat_utility(int current, char *previous, struct options *options,
                 int *printed, int *index) {
  if (!(options->s && *previous == '\n' && current == '\n' && *printed == 1)) {
    if ((*previous == '\n') && (current == '\n'))
      *printed = 1;
    else
      *printed = 0;

    if (((options->n && !options->b) || (options->n && options->b)) &&
        *previous == '\n') {
      *index += 1;
      printf("%6d\t", *index);
    }

    if ((!options->n && options->b) && *previous == '\n' && current != '\n') {
      *index += 1;
      printf("%6d\t", *index);
    }

    if (options->t && current == '\t') {
      printf("^");
      current = '\t' + 64;
    }

    if (options->e && current == '\n') {
      if ((options->n || options->b) && *previous == '\n')
        printf("%9c", '$');
      else
        printf("$");
    }

    if (options->v) {
      if (current > 127 && current < 255) {
        printf("M-^");
        printf("%c", (current & (~current + 1)));
      }
      if ((current < 32 && current != '\n' && current != '\t') ||
          current == 127)
        printf("^M");
    }
    fputc(current, stdout);
  }
  *previous = current;
}

void print_file(int count_files, char **files, struct options *options) {
  int out;
  char prev = '\n';
  int index = 0;
  int printed = 0;

  for (int i = 1; i < count_files; i++) {
    if (!(files[i][0] == '-' || strcmp(files[i], "--") == 0)) {
      FILE *file = fopen(files[i], "r");

      if (file != NULL) {
        while ((out = fgetc(file)) != EOF) {
          cat_utility(out, &prev, options, &printed, &index);
        }
      } else {
        print_error();
        break;
      }
      fclose(file);
      index = 0;
      prev = '\n';
    }
  }
}

void print_error() { printf("s21_cat error!"); }