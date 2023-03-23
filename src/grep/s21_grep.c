#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct flags {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;  // -

  int optCount;  // it is count of '-' flag needed to count files

  int size_buf;

  char **pattern;  // save pattern
  int patternCount;
  int *patternPosition;
  int patPosCount;

  int filesCount;
};

int parseFlags(struct flags *F, int *argc, char **argv);
int execution(struct flags *F, int *argc, char **argv);
void printSruct(struct flags *F);
void printArgv(int *argc, char **argv);
void printError(char **argv, int i, int n);
int initStruct(struct flags *F);
int findEFflags(struct flags *F, int *argc, char **argv);

int main(int argc, char *argv[]) {
  int isWorking = 1;

  if (argc <= 2) return 0;

  struct flags F = {0};

  initStruct(&F);

  findEFflags(&F, &argc, argv);

  isWorking = parseFlags(&F, &argc, argv);

  if (isWorking) execution(&F, &argc, argv);

  // printArgv(&argc, argv);

  // printSruct(&F);

  free(F.pattern);
  free(F.patternPosition);

  return 1;
}

int execution(struct flags *F, int *argc, char **argv) {
  int isWorking = 1;
  F->filesCount = *argc - F->optCount - F->patPosCount - 2;
  regex_t regex;  // init struct for regex
  regmatch_t pmatch[1];

  char *buf = malloc(F->size_buf * sizeof(char));
  char *printFileName = malloc(F->size_buf * sizeof(char));
  char *temp = NULL;

  // pattern processing
  // *********************

  // *********************

  for (int i = 1; i < *argc; i++) {
    int goNextFile = 0;
    // chech flags / if flags - ignore
    if (argv[i][0] == '-') goNextFile = 1;
    // checking if the argument is a matched regular expression

    for (int pCount = 0; pCount <= F->patPosCount; pCount++) {
      if (i == F->patternPosition[pCount]) goNextFile = 1;
    }

    if (goNextFile) continue;

    FILE *f = fopen(argv[i], "r");

    // chech files
    // ==============================================================
    // if file not found
    if (f == NULL) {
      isWorking = 0;
      if (!F->s)
        printf("grep: %s: No such file or directory \n", argv[i]);  // flag -s
      if (f) fclose(f);
      continue;
    }

    //==============================================================
    if (F->h)
      strcpy(printFileName, "");
    else
      strcpy(printFileName, argv[i]), strcat(printFileName, ":");
    // main algoritm
    //-------------------------
    int count = 0;      // for flag -c
    int lineCount = 0;  // -n flag
    int match = 0;      //
    // get strings in file
    while (fgets(buf, F->size_buf, f) != NULL) {
      match = 0;
      lineCount++;

      // compare strings with patterns

      //******

      for (int pCount = 0; pCount <= F->patternCount; pCount++) {
        if (F->i)
          regcomp(&regex, F->pattern[pCount], REG_ICASE);
        else
          regcomp(&regex, F->pattern[pCount], 0);

        if ((regexec(&regex, buf, 1, pmatch, 0)) == 0) {
          match++;
        }
        regfree(&regex);
      }
      //******

      if ((match && !F->v) || (F->v && match == 0)) {
        if (F->l) {
          printf("%s\n", argv[i]);
          break;
        }

        if (F->c) {
          count++;
          continue;
        }

        if (F->v && F->o) continue;

        if (F->o) {
          for (int pCount = 0; pCount <= F->patternCount; pCount++) {
            if (F->i)
              regcomp(&regex, F->pattern[pCount], REG_ICASE);
            else
              regcomp(&regex, F->pattern[pCount], 0);

            temp = buf;

            while ((regexec(&regex, temp, 1, pmatch, 0)) == 0) {
              if (F->filesCount > 1) printf("%s", printFileName);
              if (F->n) printf("%d:", lineCount);

              for (int j = pmatch->rm_so; j < pmatch->rm_eo; j++) {
                printf("%c", temp[j]);
              }
              temp = temp + pmatch->rm_eo;
              printf("\n");
            }
            regfree(&regex);
          }
          continue;
        }

        if (F->filesCount > 1) printf("%s", printFileName);

        if (F->n) {
          printf("%d:", lineCount);
        }

        // print string

        for (size_t i = 0; i < strlen(buf); i++) {
          if (buf[i] == '\n') continue;
          printf("%c", buf[i]);
        }
        printf("%s", "\n");
      }
    }

    if (F->c && !F->l) {
      if (F->filesCount > 1)
        printf("%s%d\n", printFileName, count);
      else
        printf("%d\n", count);
    }

    //-------------------------

    // if stream is open => close stream
    if (f) fclose(f);
  }

  free(buf);
  free(printFileName);

  return isWorking;
}

// fills the flags structure and outputs errors
int parseFlags(struct flags *F, int *argc, char **argv) {
  // return 1 if secsess parse or not find any flags / 0 if error for parse
  int result = 1;
  int catchPattern = 0;
  char *buf = malloc(F->size_buf * sizeof(char));

  // look all options argv for find flags
  for (int i = 1; i < *argc; i++) {
    if (argv[i][0] == '-') {  // i string in argv
      F->optCount++;
      for (size_t n = 1; n < strlen(argv[i]); n++) {
        if (result == 0) {
          break;
        }
        if (argv[i][n] == 'e') {
          if ((i + 1) > *argc - 1) {
            printf(
                "grep: option requires an argument -- 'e'\nUsage: grep "
                "[OPTION]... PATTERNS [FILE]...\nTry 'grep --help' for more "
                "information.\n");
            result = 0;
            break;
          } else {
            F->e = 1;
            F->patternCount++;
            F->patPosCount++;
            strcpy(F->pattern[F->patternCount], argv[i + 1]);
            F->patternPosition[F->patPosCount] = i + 1;
            continue;
          }
        } else if (argv[i][n] == 'f') {
          // not work
          if ((i + 1) > *argc - 1) {
            printf(
                "grep: option requires an argument -- 'f'\nUsage: grep "
                "[OPTION]... PATTERNS [FILE]...\nTry 'grep --help' for more "
                "information.\n");
            result = 0;
            break;
          } else {
            F->f = 1;
            F->patPosCount++;
            F->patternPosition[F->patPosCount] = i + 1;
            FILE *f = fopen(argv[i + 1], "r");
            // chech files
            // ==============================================================
            // if file not found
            if (f == NULL) {
              if (!F->s)
                printf("grep: %s: No such file or directory \n",
                       argv[i + 1]);  // flag -s
              if (f) fclose(f);
              result = 0;
              break;
            }
            // if file is empty
            if (getc(f) == EOF) {
              if (f) fclose(f);
              continue;
            } else {
              rewind(f);
              // add pattern in list
              while (fgets(buf, F->size_buf, f) != NULL) {
                F->patternCount++;
                if (buf[strlen(buf) - 1] == '\n') {
                  buf[strlen(buf) - 1] = '\0';
                }
                strcpy(F->pattern[F->patternCount], buf);
              }
            }
            //==============================================================
            if (f) fclose(f);
            break;
          }
        } else if (argv[i][n] == 'i') {
          F->i = 1;
        } else if (argv[i][n] == 'v') {
          F->v = 1;
        } else if (argv[i][n] == 'c') {
          F->c = 1;
        } else if (argv[i][n] == 'l') {
          F->l = 1;
        } else if (argv[i][n] == 'n') {
          F->n = 1;
        } else if (argv[i][n] == 'h') {
          F->h = 1;
        } else if (argv[i][n] == 's') {
          F->s = 1;
        } else if (argv[i][n] == 'o') {
          F->o = 1;
        } else {
          result = 0, printError(argv, i, n);
        }
        // first is pattern
      }
    } else if (!catchPattern && !F->e && !F->f) {
      F->patternCount++;
      F->patPosCount++;
      catchPattern = 1;
      strcpy(F->pattern[F->patternCount], argv[i]);
      F->patternPosition[F->patPosCount] = i;
    }
  }
  free(buf);
  return result;
}

int findEFflags(struct flags *F, int *argc, char **argv) {
  // return 1 if secsess parse or not find any flags / 0 if error for parse
  int result = 1;
  // look all options argv for find flags -e -f
  for (int i = 1; i < *argc; i++) {
    if (argv[i][0] == '-') {  // i string in argv
      for (size_t n = 1; n < strlen(argv[i]); n++) {
        if (result == 0) {
          break;
        }
        if (argv[i][n] == 'e') F->e = 1;
        if (argv[i][n] == 'f') F->f = 1;
        if (argv[i][n] == 's') F->s = 1;
      }
    }
  }
  return result;
}

int initStruct(struct flags *F) {
  int isInit = 1;
  F->size_buf = 1024;

  F->pattern = malloc(F->size_buf * F->size_buf * sizeof(char));
  char *pattern_ptr = (char *)(F->pattern + F->size_buf);
  for (int i = 0; i < F->size_buf; i++) {
    F->pattern[i] = pattern_ptr + F->size_buf * i;
  }
  F->patternPosition = malloc(F->size_buf * sizeof(int));
  F->patternCount = -1;
  F->patPosCount = -1;

  return isInit;
}

void printSruct(struct flags *F) {
  printf("\e[1;91mstruct F:\t");
  printf(
      " e = %d\t i = %d\t v = %d\t c = %d\t l = %d\t n = %d\t h = %d\t s = "
      "%d\t "
      "f = %d\t o = %d\n",
      F->e, F->i, F->v, F->c, F->l, F->n, F->h, F->s, F->f, F->o);

  for (int pCount = 0; pCount <= F->patternCount; pCount++) {
    printf("patternCount %d\t", pCount);
    printf("%s\n", F->pattern[pCount]);
  }

  printf("optCount = %d\t", F->optCount);
  printf("fileCount = %d\t", F->filesCount);
  printf("patPosCount = %d\t", F->patPosCount);
  printf("paternCount = %d\e[0m\n", F->patternCount);
}

void printArgv(int *argc, char **argv) {
  // test init param
  for (int i = 0; i < *argc; i++) {
    printf("\e[1;91margc = %d argv = %s\e[0m\n", i, argv[i]);
  }
}

void printError(char **argv, int i, int n) {
  printf("cat: invalid option -- '%c'\n", argv[i][n]),
      printf("Usage: grep [OPTION]... PATTERNS [FILE]...\n"),
      printf("Try 'grep --help' for more information.\n");
}

/*
Синтаксис команды выглядит следующим образом:

$ grep [опции] шаблон [имя файла...]

Или:

$ команда | grep [опции] шаблон

Опции - это дополнительные параметры, с помощью которых указываются различные
настройки поиска и вывода, например количество строк или режим инверсии.
Шаблон
- это любая строка или регулярное выражение, по которому будет вестись поиск
Файл и команда - это то место, где будет вестись поиск. Как вы увидите дальше,
grep позволяет искать в нескольких файлах и даже в каталоге, используя
рекурсивный режим. Возможность фильтровать стандартный вывод
пригодится,например, когда нужно выбрать только ошибки из логов или найти PID
процесса в многочисленном отчёте утилиты ps.
1 -e Шаблон
2 -i Игнорирует различия регистра.
3 -v Инвертирует смысл поиска соответствий.
4 -c Выводит только количество совпадающих строк.
5 -l Выводит только совпадающие файлы.
6 -n Предваряет каждую строку вывода номером строки из файла ввода.
7 -h Выводит совпадающие строки, не предваряя их именами файлов.
8 -s Подавляет сообщения об ошибках о несуществующих или нечитаемых файлах.
9 -f file Получает регулярные выражения из файла.
10 -o Печатает только совпадающие (непустые) части совпавшей строки.

       grep [OPTION...] PATTERNS [FILE...]
       grep [OPTION...] -e PATTERNS ... [FILE...]
       grep [OPTION...] -f PATTERN_FILE ... [FILE...]

*/
