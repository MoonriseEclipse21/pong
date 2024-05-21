#include "s21_cat.h"

int main(int argc,  char** argv){
    Flags flags = parser(argc, argv);
    if (argc == 1){
        output_file(stdin);
    }
    for (int i = optind; i < argc; i++){
        output_file_flags(&flags, argv[i]);
    }
    return 0;
}

Flags parser(int argc, char** argv){
    Flags flag = {0};
    struct option long_option[] = {
        {"number-nonblank", 0, NULL, 'b'},
        {"number", 0, NULL, 'n'},
        {"squeeze-blank", 0, NULL, 's'},
        {0, 0, 0, 0}
    };
    int current_flag = getopt_long(argc, argv, "beEvnstT", long_option, 0);
    while (current_flag != -1){
        switch (current_flag){
            case 'b':
            flag.b = 1;
            break;
            case 'e':
            flag.e = 1;
            flag.v = 1;
            break;
            case 'E':
            flag.E = 1;
            break;
            case 'v':
            flag.v = 1;
            break;
            case 'n':
            flag.n = 1;
            break;
            case 's':
            flag.s = 1;
            break;
            case 't':
            flag.t = 1;
            flag.v = 1;
            break;
            case 'T':
            flag.T = 1;
            break;
            case '?':
            perror("error");
            exit(1);
            break;
            default:
            perror("error");
            exit(2);
            break;

        };
        current_flag = getopt_long(argc, argv, "beEvnstT", long_option, 0);
    }
    return flag;
}
char flag_v(unsigned char ch) {
  if (ch != '\n' && ch != '\t') {
    if (ch <= 31) {
      putchar('^');
      ch += 64;
    } else if (ch == 127) {
      putchar('^');
      ch = '?';
    } else if (ch >= 128 && ch < 161) {
      printf("M-^");
      ch -= 64;
    }
  }
  return ch;
}

void flags_E_T_v(Flags* flag, char* line_text, int n) {
  for (int i = 0; i < n; i++) {
    if ((flag->E || flag->e) && line_text[i] == '\n') {
      putchar('$');
    }
    if (flag->v) {
      line_text[i] = flag_v(line_text[i]);
    }
    if ((flag->T || flag->t) && line_text[i] == '\t') {
      putchar('^');
      line_text[i] = 'I';
    }
    putchar(line_text[i]);
  }
}

void output_file_flags(Flags* flag, const char* argv) {
  FILE* f = fopen(argv, "r");
  if (f == NULL) {
    perror("ERROR");
    exit(1);
  }

  char* text_line = NULL;
  size_t storing_text = 0;
  int read_getline = getline(&text_line, &storing_text, f);

  int number_of_line = 1;
  int number_empty_line = 0;
  int number_nonempty_line = 1;
  while (read_getline != -1) {
    if (text_line[0] == '\n') {
      number_empty_line += 1;
    } else {
      number_empty_line = 0;
    }
    if (flag->s && number_empty_line > 1) {
    } else {
      if (flag->n && !flag->b) {
        printf("%6d\t", number_of_line);
        number_of_line++;
      } else if (flag->b && text_line[0] != '\n') {
        printf("%6d\t", number_nonempty_line);
        number_nonempty_line++;
      }
      flags_E_T_v(flag, text_line, read_getline);
    }
    read_getline = getline(&text_line, &storing_text, f);
  }
  free(text_line);
  fclose(f);
}

void output_file(FILE* fp) {
  char c;
  while ((c = fgetc(fp)) != EOF) fputc(c, stdout);
}
