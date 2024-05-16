#ifndef SRC_CAT_S21_CAT_H_
#define SRC_CAT_S21_CAT_H_

#include <stdio.h>
#include <string.h>

typedef struct options {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
  int E;
  int T;
  char prev;
  int line_num;
} options_cat;

void opt_n(options_cat *opt);
void opt_b(char c, options_cat *opt);
void opt_v(char *c);
void opt_v(char *c);
void opt_t(char *c);
void opt_e(char c);
void skip_empty_lines(char *c, FILE *fp, options_cat *opt);
void clean_struct(options_cat *opt);
void s21_cat(int argc, char *argv[]);
void pars_options(int argc, char *argv[]);

#endif  // SRC_CAT_S21_CAT_H_
