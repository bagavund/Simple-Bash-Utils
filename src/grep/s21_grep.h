#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_

#include <glob.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct grep_options {
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int o;
} grep_opt;

void match_finder(grep_opt opt, regex_t *regex_arr, int len, int argc,
                  char **argv, int current_position);
regex_t *regex_e_plus_regex_f(regex_t *regex_e, regex_t *regex_from_file,
                              int e_cnt, int f_cnt);
regex_t *pars_options_e(int argc, char *argv[], int *e_n, const int *err,
                        grep_opt opt);
regex_t *pars_options_f(int argc, char *argv[], int *f_n, int *err,
                        grep_opt opt);
regex_t *to_regex_compile(int *current_position, int argc, char *argv[],
                          regex_t *regex_arr, int *len, grep_opt opt);
regex_t *s21_get_regex_from_file(char **file_names, int len, int *f_n,
                                 int *error, grep_opt grep_set);
void preparsing_options(int n, char *argv[], int *err, grep_opt *opt);
char *s21_string_check_len_and_allocate_mem(char *str);

#endif  // SRC_GREP_S21_GREP_H_

