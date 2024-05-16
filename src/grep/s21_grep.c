#include "s21_grep.h"

int main(int argc, char *argv[]) {
  grep_opt opt = {0};
  int err = (argc < 3);
  int e_n = 0, f_n = 0;
  int current_position = 1;
  preparsing_options(argc, argv, &err, &opt);
  regex_t *regex_arr = pars_options_e(argc, argv, &e_n, &err, opt);
  regex_t *regex_from_file = pars_options_f(argc, argv, &f_n, &err, opt);
  int len = e_n + f_n;
  regex_arr = regex_e_plus_regex_f(regex_arr, regex_from_file, e_n, f_n);
  if (!e_n && !f_n) {
    regex_arr =
        to_regex_compile(&current_position, argc, argv, regex_arr, &len, opt);
  }
  if (!err) {
    match_finder(opt, regex_arr, len, argc, argv, current_position);
  }
  if (regex_arr != NULL) {
    free(regex_arr);
  }

  return 0;
}

void preparsing_options(int n, char *argv[], int *err, grep_opt *opt) {
  for (int i = 1; i < n && !(*err); i++) {
    if (*(argv[i]) == '-' && strcmp(argv[i], "-e") && strcmp(argv[i], "-f")) {
      if (strlen(argv[i]) == 1) {
        break;
      }
      for (size_t k = 1; k < strlen(argv[i]); k++) {
        switch ((argv[i][k])) {
          case 'i':
            opt->i = 1;
            break;
          case 'v':
            opt->v = 1;
            break;
          case 'c':
            opt->c = 1;
            break;
          case 'l':
            opt->l = 1;
            break;
          case 'n':
            opt->n = 1;
            break;
          case 'h':
            opt->h = 1;
            break;
          case 's':
            opt->s = 1;
            break;
          case 'o':
            opt->o = 1;
            break;
          default:
            printf("s21_grep: illegal option -- %s\n", (argv[i] + 1));
            printf("usage: ./s21_grep [-eivclnhsfo] [file ...]");
            *err = 1;
            break;
        }
      }
    }
  }
}

regex_t *pars_options_e(int argc, char *argv[], int *e_n, const int *err,
                        grep_opt opt) {
  int counter = 0;
  regex_t *regex_arr = NULL;
  int i = 1;
  while (i < argc && !(*err)) {
    if (!strcmp(argv[i], "-e")) {
      counter++;
      i++;
      if (regex_arr == NULL) {
        regex_arr = malloc(0);
      }
      regex_t *tmp = realloc(regex_arr, counter * sizeof(regex_t));
      if (tmp != NULL) {
        regex_arr = tmp;
      }
      char *tmp_str = malloc((strlen(argv[i]) + 1) * sizeof(char));
      strcpy(tmp_str, argv[i]);
      tmp_str = s21_string_check_len_and_allocate_mem(tmp_str);
      if (opt.i) {
        regcomp(regex_arr + (counter - 1), tmp_str, REG_ICASE);
      } else {
        regcomp(regex_arr + (counter - 1), tmp_str, 0);
      }
      free(tmp_str);
    }
    i++;
  }
  *e_n = counter;
  return regex_arr;
}

regex_t *pars_options_f(int argc, char *argv[], int *f_n, int *err,
                        grep_opt opt) {
  int i = 1;
  int counter = 0;
  char **file_names = NULL;

  while (i < argc && !(*err)) {
    if (!strcmp(argv[i], "-f") && i != argc - 1) {
      i++;
      counter++;
      if (file_names == NULL) {
        file_names = malloc(counter * sizeof(char *));
      } else {
        char **tmp = realloc(file_names, counter * sizeof(char *));
        if (tmp != NULL) {
          file_names = tmp;
        }
      }
      *(file_names + counter - 1) =
          malloc((strlen(argv[i]) + 1) * sizeof(char));
      strcpy(*(file_names + counter - 1), argv[i]);
    }
    i++;
  }
  int file_len = counter;
  regex_t *ret = s21_get_regex_from_file(file_names, file_len, f_n, err, opt);
  for (int k = 0; k < counter; k++) {
    free(*(file_names + k));
  }
  if (file_names != NULL) {
    free(file_names);
  }

  return ret;
}

regex_t *regex_e_plus_regex_f(regex_t *regex_e, regex_t *regex_from_file,
                              int e_cnt, int f_cnt) {
  if (regex_e == NULL) {
    regex_e = malloc(0);
  }
  regex_e = realloc(regex_e, (e_cnt + f_cnt) * sizeof(regex_t));
  for (int i = 0; i < f_cnt; i++) {
    regex_e[e_cnt + i] = regex_from_file[i];
  }
  if (regex_from_file != NULL) {
    free(regex_from_file);
  }
  return regex_e;
}

regex_t *to_regex_compile(int *current_position, int argc, char *argv[],
                          regex_t *regex_arr, int *len, grep_opt opt) {
  regex_t *ret_reg = regex_arr;
  while (*current_position < argc - 1) {
    if (argv[*current_position][0] != '-') {
      char *tmp_str =
          malloc((strlen(argv[*current_position]) + 1) * sizeof(char));
      strcpy(tmp_str, argv[*current_position]);
      tmp_str = s21_string_check_len_and_allocate_mem(tmp_str);
      (*len)++;
      if (*len == 1) {
        ret_reg = malloc((*len) * sizeof(regex_t));
      } else {
        ret_reg = realloc(ret_reg, *len * sizeof(regex_t));
      }
      if (opt.i) {
        regcomp(ret_reg + (*len - 1), tmp_str, REG_ICASE);
      } else {
        regcomp(ret_reg + (*len - 1), tmp_str, 0);
      }
      free(tmp_str);
      *current_position = *current_position + 1;
      break;
    }
    *current_position = *current_position + 1;
  }
  return ret_reg;
}

void match_finder(grep_opt opt, regex_t *regex_arr, int len, int argc,
                  char **argv, int current_position) {
  int mul_files = 0;

  for (int i = current_position; i < argc; i++) {
    if (strchr(argv[i], '-') != NULL) {
      if (!strcmp(argv[i], "-f") || !strcmp(argv[i], "-e")) {
        i++;
      }
      continue;
    }
    mul_files++;
  }

  for (int i = current_position; i < argc; i++) {
    if (strchr(argv[i], '-') != NULL) {
      if (!strcmp(argv[i], "-f") || !strcmp(argv[i], "-e")) {
        i++;
      }
      continue;
    }
    FILE *f = fopen(argv[i], "r");
    if (f != NULL) {
      size_t nmatch = 2;
      regmatch_t pmatch[2];
      char c;
      int str_len = 0;
      int str_num = 0;
      int match_counter = 0;
      char *str = malloc(1);
      int on = 1;
      int matched = 0;
      while (on) {
        int scanning = fscanf(f, "%c", &c);
        if (c == '\n') {
          c = '\0';
        } else if (scanning == -1) {
          on = 0;
          c = '\0';
        }
        str_len++;
        char *tmp = realloc(str, str_len * sizeof(char));
        if (tmp != NULL) {
          str = tmp;
          *(str + str_len - 1) = c;
        }
        if ((on && c == '\0') || (!on && *str != '\0')) {
          str_num++;
          matched = 0;
          for (int k = 0; k < len; k++) {
            int ret = regexec(regex_arr + k, str, nmatch, pmatch, 0);
            if (!ret) {
              matched = 1;
              break;
            }
          }
          if (opt.v) {
            matched = matched ? 0 : 1;
          }
          if (matched) {
            match_counter++;
            if (opt.l) {
              break;
            }
            if (!opt.c) {
              if (mul_files > 1 && !opt.h) {
                printf("%s:", argv[i]);
              }
              if (opt.n) {
                printf("%d:", str_num);
              }
              if (opt.o && !opt.v) {
                int appender = pmatch[0].rm_eo;
                printf("%.*s\n", (int)(pmatch[0].rm_eo - pmatch[0].rm_so),
                       &str[pmatch[0].rm_so]);
                for (int k = 0; k < len; k++) {
                  while (!regexec(regex_arr + k, str + appender, nmatch, pmatch,
                                  REG_NOTBOL) &&
                         pmatch[0].rm_eo != pmatch[0].rm_so) {
                    printf("%.*s\n", (int)(pmatch[0].rm_eo - pmatch[0].rm_so),
                           &str[appender + pmatch[0].rm_so]);
                    appender += pmatch[0].rm_eo;
                  }
                }
              } else {
                printf("%s\n", str);
              }
            }
          }

          free(str);
          str_len = 0;
          str = malloc(0);
        }
      }
      if (opt.c) {
        if (mul_files > 1 && !opt.h) {
          printf("%s:", argv[i]);
        }
        printf("%d\n", match_counter);
      }
      if (opt.l && matched) {
        printf("%s\n", argv[i]);
      }
      free(str);
    } else {
      printf("s21_grep:No such file or directory:%s\n", argv[i]);
    }
  }
  for (int k = 0; k < len; k++) {
    regfree(regex_arr + k);
  }
}

char *s21_string_check_len_and_allocate_mem(char *str) {
  char *ret = NULL;
  if (str != NULL) {
    int n = strlen(str);
    if (*str == '\'' && *(str + n - 1) == '\'' && n > 1) {
      ret = malloc(n - 1);
      strncpy(ret, str + 1, n - 2);
      free(str);
    } else if (*str == '\"' && *(str + n - 1) == '\"' && n > 1) {
      ret = malloc(n - 1);
      strncpy(ret, str + 1, n - 2);
      free(str);
    } else {
      ret = str;
    }
  }
  return ret;
}

regex_t *s21_get_regex_from_file(char **file_names, int len, int *f_n,
                                 int *error, grep_opt opt) {
  regex_t *regex_arr = NULL;

  for (int i = 0; i < len && !(*error); i++) {
    FILE *f = fopen(file_names[i], "r");
    if (f == NULL && opt.s) {
      *error = 1;
      printf("No such file or directory:%s\n", file_names[i]);
    } else {
      int str_len = 0;
      char *str = malloc(1);
      int on = 1;
      while (on) {
        int c;
        c = fgetc(f);
        if (c == '\n') {
          c = '\0';
        } else if (c == EOF) {
          on = 0;
          c = '\0';
        }
        str_len++;
        char *tmp = realloc(str, str_len * sizeof(char));
        if (tmp != NULL) {
          str = tmp;
          *(str + str_len - 1) = c;
        }
        if (c == '\0') {
          str = s21_string_check_len_and_allocate_mem(str);
          (*f_n)++;
          if (regex_arr == NULL) {
            regex_arr = malloc(0);
          }
          regex_t *tmp_regex = realloc(regex_arr, (*f_n) * sizeof(regex_t));
          if (tmp_regex != NULL) {
            regex_arr = tmp_regex;
          }
          if (opt.i) {
            regcomp(regex_arr + *f_n - 1, str, REG_ICASE);
          } else {
            regcomp(regex_arr + (*f_n) - 1, str, 0);
          }
          free(str);
          str_len = 0;
          str = malloc(0);
        }
      }
      free(str);
      fclose(f);
    }
  }

  return regex_arr;
}

