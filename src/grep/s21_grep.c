#include "s21_grep.h"

int main(int argc, char **argv) {
  int error_code = 0;

  parser(argc, argv, &error_code);
  files_parser(argc, argv, &error_code);

  if (!error_code) {
    grep_output(files, patterns, flags);
  } else {
    printf("Error\n");
  }

  free_all();
  return 0;
}

void parser(int arg_count, char **arg_array, int *error_code) {
  int flag;
  while ((flag = getopt_long(arg_count, arg_array, "e:ivclnhsf:o", NULL,
                             NULL)) != -1) {
    switch (flag) {
      case 'e':
        flags.regex |= REG_EXTENDED;
        add_pattern('e', optarg, error_code);
        break;
      case 'i':
        flags.regex |= REG_ICASE;
        break;
      case 'v':
        flags.invert = 1;
        break;
      case 'c':
        flags.count = 1;
        break;
      case 'l':
        flags.files = 1;
        break;
      case 'n':
        flags.line_num = 1;
        break;
      case 'h':
        flags.no_file_name = 1;
        break;
      case 's':
        flags.error_null = 1;
        break;
      case 'o':
        flags.match_part = 1;
        break;
      case 'f':
        flags.file_pattern = 1;
        add_pattern('f', optarg, error_code);
        break;
      default:
        *error_code = 1;
        break;
    }
  }
}

void files_parser(int arg_count, char **arg_array, int *error_code) {
  if (patterns.patterns == NULL) {
    add_pattern('e', arg_array[optind], error_code);
    optind++;
  }

  for (int i = optind; i < arg_count; i++) {
    if (arg_array[i][0] == '-' || strstr(arg_array[i - 1], "-e") ||
        strstr(arg_array[i - 1], "-f")) {
      continue;
    }
    files.count_files++;
    files.files = realloc(files.files, files.count_files * sizeof(char *));
    files.files[files.count_files - 1] = malloc(strlen(arg_array[i]) + 1);
    strcpy(files.files[files.count_files - 1], arg_array[i]);
  }
}

void add_pattern(char flag, char *optarg, int *error_code) {
  int exist = 0;
  for (int i = 0; i < patterns.count_patterns; i++) {
    if (strcmp(patterns.patterns[i], optarg) == 0) {
      exist = 1;
      break;
    }
  }
  if (!exist && flag == 'e') {
    patterns.count_patterns++;
    patterns.patterns =
        realloc(patterns.patterns, patterns.count_patterns * sizeof(char *));
    patterns.patterns[patterns.count_patterns - 1] = malloc(strlen(optarg) + 1);
    strcpy(patterns.patterns[patterns.count_patterns - 1], optarg);
  }

  if (flag == 'f') {
    FILE *file = fopen(optarg, "r");
    char *line = NULL;
    size_t len = 0;

    if (file != NULL) {
      while ((getline(&line, &len, file)) != -1) {
        add_pattern('e', line, error_code);
        free(line);
        line = NULL;
      }
      free(line);
      fclose(file);
    } else {
      *error_code = 1;
    }
  }
}

void grep_output(struct files files, struct patterns patterns,
                 struct options flags) {
  for (int i = 0; i < files.count_files; i++) {
    FILE *file = fopen(files.files[i], "r");
    if (file == NULL) {
      if (flags.error_null) continue;
      printf("Error fopen\n");
      continue;
    }
    grep_utility(file, files.files[i], flags, patterns);
    fclose(file);
  }
}

void grep_utility(FILE *file, char *filename, struct options flags,
                  struct patterns patterns) {
  regex_t preg;
  regmatch_t match;
  char *line = NULL;
  size_t len = 0;
  int count = 0;
  int found = 0;
  int match_count = 0;
  int print_status = 0;

  while ((getline(&line, &len, file)) != -1) {
    count++;
    for (int i = 0; i < patterns.count_patterns; i++) {
      if (!regcomp(&preg, patterns.patterns[i], flags.regex) && !print_status) {
        if (flags.match_part && !flags.count && !flags.files && !flags.invert) {
          grep_match_part(line, filename, count, &preg);
        } else if (regexec(&preg, line, 1, &match, flags.regex) ==
                   flags.invert) {
          found = 1;
          match_count++;
          print_status = 1;
          if (!flags.count && !flags.files) {
            grep_print(line, filename, count, flags);
          }
        }
      }
      regfree(&preg);
    }
    print_status = 0;
    free(line);
    line = NULL;
  }
  if (flags.files && found) {
    grep_files_print(filename, flags);
  }
  if (flags.count && found) {
    grep_count(match_count, filename, flags);
  }
  free(line);
}

void grep_count(int match_count, char *filename, struct options flags) {
  if ((files.count_files > 1 || flags.files) && !flags.no_file_name) {
    printf("%s:", filename);
  }
  printf("%d\n", match_count);
}

void grep_files_print(char *filename, struct options flags) {
  if (files.count_files > 1 || flags.files) {
    printf("%s\n", filename);
  }
}

void grep_print(char *line, char *filename, int count, struct options flags) {
  if ((files.count_files > 1 || flags.files) && !flags.no_file_name) {
    printf("%s:", filename);
  }
  if (flags.line_num) {
    printf("%d:", count);
  }
  printf("%s", line);
}

void grep_match_part(char *line, char *filename, int count, regex_t *preg) {
  const char *ptr = line;
  regmatch_t match;

  while (regexec(preg, ptr, 1, &match, (ptr == line) ? 0 : REG_NOTBOL) == 0) {
    int print_length = match.rm_eo - match.rm_so;

    if (files.count_files > 1 && !flags.no_file_name) {
      printf("%s:", filename);
    }
    if (flags.line_num) {
      printf("%d:", count);
    }

    printf("%.*s\n", print_length, ptr + match.rm_so);
    ptr += match.rm_eo;
  }
}

void free_all() {
  for (int i = 0; i < files.count_files; i++) {
    free(files.files[i]);
  }
  free(files.files);

  for (int i = 0; i < patterns.count_patterns; i++) {
    free(patterns.patterns[i]);
  }
  free(patterns.patterns);
}