#include "s21_cat.h"

int main(int argc, char *argv[]) {
  pars_options(argc, argv);
  return 0;
}

void skip_empty_lines(char *c, FILE *fp, options_cat *opt) {
  while (*c == '\n' && opt->prev == '\n') {
    opt->prev = *c;
    *c = fgetc(fp);
    if (*c != '\n' && opt->prev == '\n') {
      if (opt->n == 1 && opt->b == 0) opt_n(opt);
      if (opt->e == 1) printf("$");
      printf("\n");
    }
  }
}

void opt_n(options_cat *opt) {
  if (opt->prev == '\n' || opt->prev == -1) printf("%6d\t", opt->line_num++);
}

void opt_b(char c, options_cat *opt) {
  if ((opt->prev == '\n' && c != '\n') || (opt->prev == -1 && c != '\n'))
    printf("%6d\t", opt->line_num++);
}

void opt_v(char *c) {
  if ((*c >= 0 && *c <= 31 && *c != 9 && *c != 10) || *c == 127) {
    printf("^");
    if (*c >= 0 && *c <= 31)
      *c += '@';
    else if (*c == 127)
      *c -= '@';
  }
}

void opt_t(char *c) {
  if (*c == 9 || *c == 10) {
    printf("^");
    *c += '@';
  }
}

void opt_e(char c) {
  if (c == '\n') printf("$");
}

void print_with_options(char *argv[], int i, options_cat *opt) {
  FILE *fp;
  char c;
  if ((fp = fopen(argv[i], "r")) == NULL) {
    printf("s21_cat: %s: No such file or directory\n", argv[i]);
  } else {
    while ((c = fgetc(fp)) != EOF) {
      if (opt->s == 1) {
        skip_empty_lines(&c, fp, opt);
      }
      if (opt->n == 1 && opt->b == 0) opt_n(opt);
      if (opt->b == 1) opt_b(c, opt);
      if (opt->v == 1 && c != '\n') opt_v(&c);
      if ((opt->t == 1 || opt->T == 1) && c != '\n') opt_t(&c);
      if (opt->e == 1 || opt->E == 1) opt_e(c);
      printf("%c", c);
      opt->prev = c;
    }
  }
}

void lenght_flag(char *argv[], int i, int j, options_cat *opt) {
  if (argv[i][j] == '-') {
    printf(
        "s21_cat: illegal option -- -\nusage: s21_cat [-benstuv] [file ...]\n");
  }
  if (!strcmp(argv[i], "--number-nonblank"))
    opt->b = 1;
  else if (!strcmp(argv[i], "--number"))
    opt->n = 1;
  else if (strcmp(argv[i], "--squeeze-blank"))
    opt->s = 1;
}

void pars_options(int argc, char *argv[]) {
  int i = 1;
  options_cat opt = {0};
  opt.prev = -1;
  opt.line_num = 1;
  while ((argc) != i) {
    int j = 0;
    if (argv[i][j] == '-') {
      j++;
      if (argv[i][j] == '-') {
        j++;
        if (argv[i][j] == 'n' || argv[i][j] == 's') {
          lenght_flag(argv, i, j, &opt);
        } else {
          printf(
              "s21_cat: illegal option -- -\nusage: s21_cat [-benstuv] [file "
              "...]\n");
          break;
        }
      } else {
        while (argv[i][j]) {
          if (argv[i][j] == 'b') opt.b = 1;
          if (argv[i][j] == 'e') {
            opt.e = 1;
            opt.v = 1;
          }
          if (argv[i][j] == 'n') opt.n = 1;
          if (argv[i][j] == 's') opt.s = 1;
          if (argv[i][j] == 't') {
            opt.t = 1;
            opt.v = 1;
          }
          if (argv[i][j] == 'v') opt.v = 1;
          if (argv[i][j] == 'E') opt.E = 1;
          if (argv[i][j] == 'T') opt.T = 1;
          j++;
        }
      }
    } else {
      print_with_options(argv, i, &opt);
      opt.line_num = 1;
      opt.prev = -1;
    }
    i++;
  }
}
