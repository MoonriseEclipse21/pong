#include "grep.h"

int main(int argc, char* argv[]) {
  if (argc > 1) {
    flags flag = parser(argc, argv);
    reg_comp(flag, argc, argv);
    free(flag.pattern);
  }
  return 0;
}

flags parser(int argc, char** argv) {
  flags flag = {0};
  int current_flag = getopt(argc, argv, "e:ivclnhsf:o");
  while ((current_flag) != -1) {
    switch (current_flag) {
      case 'e':
        flag.e = 1;
        add_pattern(&flag, optarg);
        break;
      case 'i':
        flag.i = REG_ICASE;
        break;
      case 'v':
        flag.v = 1;
        break;
      case 'c':
        flag.c = 1;
        break;
      case 'l':
        flag.l = 1;
        flag.c = 1;
        break;
      case 'n':
        flag.n = 1;
        break;
      case 'h':
        flag.h = 1;
        break;
      case 's':
        flag.s = 1;
        break;
      case 'f':
        flag.f = 1;
        flag_f(&flag, optarg);
        break;
      case 'o':
        flag.o = 1;
        break;
      case '?':
        perror("error");
        exit(1);
      default:
        perror("error");
        exit(2);
    }
    current_flag = getopt(argc, argv, "e:ivclnhsf:o");
  }
  if (flag.lenght_of_pattern == NULL){ 
    add_pattern(&flag, argv[optind++]);
  }
  if (argc - optind == 1) {
    flag.h = 1;}
  return flag;
}

void reg_comp(flags flag, int argc, char** argv) {
  regex_t reg;

  int error = regcomp(&reg, flag.pattern, REG_EXTENDED | flag.i); 
  if (error > 0) perror("ERROR");
  for (int i = optind; i < argc; i++) {
   processing_file_pattens(flag, argv[i], &reg);
  }
  regfree(&reg);
}

void processing_file_pattens(flags flag, char* path, regex_t* reg) {
  FILE* f = fopen(path, "r");
  if (f == NULL) {
    if (!flag.s) perror("error");
    exit(1);
  }

  char* file_text = NULL;
  size_t memory_text = 0;
  int read_textline = getline(&file_text, &memory_text, f);
  int count_line = 1;
  int match_count = 0;
  while (read_textline != -1) {


int result = regexec(reg, file_text, 0, NULL, 0);
    if ((result == 0 && !flag.v) || (result != 0 && flag.v)) {
      if (!flag.h) printf("%s:", path);
      if (!flag.c && !flag.l) {
        if (flag.n) printf("%d:", count_line);
        if (flag.o)
          flag_o(reg, file_text);
        else
          output_text(file_text, read_textline);
      }
      match_count++;
    }
    if (flag.n) count_line++;
    read_textline = getline(&file_text, &memory_text, f);
  }
  if (flag.c && !flag.l) printf("%d\n", match_count);
  if (flag.l && match_count > 0) printf("%s\n", path);
  free(file_text);
  fclose(f);
}

void add_pattern(flags* flag, char* pattern) {  // pattern == optarg
  if (pattern == NULL) {
    return;
  }
  int len_pat = strlen(pattern);
  len_pat += flag->memory_for_pattern;
  if (flag->lenght_of_pattern == 0) {
    flag->pattern = malloc(1000 * sizeof(char));
    if (flag->pattern == NULL) {
      perror("Can't allocate memory");
      exit(1);
    }
    flag->pattern[0] = '\0';
    flag->memory_for_pattern = 1000;
  }
  if (flag->memory_for_pattern < len_pat) {
    flag->pattern = realloc(flag->pattern, flag->memory_for_pattern * 2);
    flag->memory_for_pattern *= 2;
  }
  if (flag->lenght_of_pattern != 0) {
    strcat(flag->pattern + flag->lenght_of_pattern, "|");
    flag->lenght_of_pattern++;
  }
  flag->lenght_of_pattern += sprintf(flag->pattern + flag->lenght_of_pattern, "(%s)", pattern);
}

void output_text(char* file_text, int n) {
  for (int i = 0; i < n; i++) printf("%c", file_text[i]);
  if (file_text[n - 1] != '\n') putchar('\n');
}

void flag_o(regex_t* reg, char* file_text) {
  regmatch_t regmatch;
  int offsets = 0;
  while (1) {
    int result = regexec(reg, file_text + offsets, 1, &regmatch, 0);
    if (result != 0) break;
    for (int i = regmatch.rm_so; i < regmatch.rm_eo; i++) {
      putchar(file_text[i]);
    }
    putchar('\n');
    offsets += regmatch.rm_eo;
  }
}

void flag_f(flags* flag, char* file_path) { 
  FILE* f = fopen(file_path, "r");
  if (f == NULL) {
    if (!flag->s) perror("error");
    exit(1);
  }
  char* file_text = NULL;
  size_t memory_text = 0;
  int read_textline = getline(&file_text, &memory_text, f);

  while (read_textline != -1) {
    if (file_text[read_textline - 1] == '\n') file_text[read_textline - 1] = '\0';
    add_pattern(flag, file_text);
    read_textline = getline(&file_text, &memory_text, f);
  }
  free(file_text);
  fclose(f);
}
