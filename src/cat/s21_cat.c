#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct flags {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
};

void stdio();
void printSruct(struct flags *F);
void printArgv(int *argc, char **argv);
int parseFlags(struct flags *F, int *argc, char **argv);
int execution(struct flags *F, int *argc, char **argv);

int main(int argc, char *argv[]) {
  int isWorking = 1;
  struct flags F = {0};
  //
  if (argc == 1) {
    stdio();
    isWorking = 0;
  }

  if (isWorking) {
    //   printArgv(&argc, argv);                   // for testing
    isWorking = parseFlags(&F, &argc, argv);  // 1 if parse seccess 0 is failed
  }

  if (isWorking) {
    execution(&F, &argc, argv);  // 1 if parse seccess 0 is
  }

  // printSruct(&F);

  return 1;
}

int execution(struct flags *F, int *argc, char **argv) {
  int isWorking = 1;

  for (int i = 1; i < *argc; i++) {
    // chech flags / if flags - ignore
    if (argv[i][0] == '-') continue;

    FILE *f = fopen(argv[i], "r");

    // chech files
    // ==============================================================
    // if file not found
    if (f == NULL) {
      isWorking = 0;
      printf("cat: %s: No such file or directory \n", argv[i]);
      if (f) fclose(f);
      continue;
    }

    // if file is empty
    if (getc(f) == EOF) {
      if (f) fclose(f);
      continue;
    } else {
      rewind(f);
    }
    //==============================================================

    // if both flags are present
    if (F->b && F->n) F->n = 0;

    char ch;
    int count = 1;
    int isThisNewLine = 1;
    int skip = 0;

    // print file
    //--------------------------
    while ((ch = getc(f)) != EOF) {
      if (F->s && isThisNewLine > 1 && ch == '\n') {
        skip = 1;
      } else {
        skip = 0;
      }

      if (F->n && isThisNewLine && !skip) {
        printf("%6d\t", count++);
      }
      if (F->b && isThisNewLine && ch != '\n') {
        printf("%6d\t", count++);
      }

      if (F->e && ch == '\n' && !skip) printf("%c", '$');

      if (F->t && ch == '\t' && !skip) {
        printf("^I");
        skip = 1;
      }

      if (F->v && ch >= 0 && ch < 32 && ch != '\n' && ch != '\t') {
        printf("^%c", 64 + ch);
        skip = 1;
      } else if (F->v && ch == 127 && ch != '\n' && ch != '\t') {
        printf("^%c", ch - 64);
        skip = 1;
      }

      // this is a new line
      if (ch == '\n') {
        isThisNewLine++;
      } else {
        isThisNewLine = 0;
      }

      if (!skip) {
        printf("%c", ch);
      }
    }

    //-------------------------

    // if stream is open => close stream
    if (f) fclose(f);
  }
  return isWorking;
}

// fills the flags structure and outputs errors
int parseFlags(struct flags *F, int *argc, char **argv) {
  // return 1 if secsess parse or not find any flags / 0 if error for parse
  int result = 1;
  // look all options argv for find flags
  for (int i = 1; i < *argc; i++) {
    // find long flags
    if (!strcmp(argv[i], "--number-nonblank")) {
      F->b = 1;
    } else if (!strcmp(argv[i], "--number")) {
      F->n = 1;
    } else if (!strcmp(argv[i], "--squeeze-blank")) {
      F->s = 1;
      // find short flags
    } else {
      if (argv[i][0] == '-') {  // i string in argv
        for (size_t n = 1; n < strlen(argv[i]); n++) {
          if (result == 0) {
            break;
          }

          if (argv[i][n] == 'b')
            F->b = 1;
          else if (argv[i][n] == 'e')
            F->e = 1, F->v = 1;
          else if (argv[i][n] == 'E')
            F->e = 1;
          else if (argv[i][n] == 'n')
            F->n = 1;
          else if (argv[i][n] == 's')
            F->s = 1;
          else if (argv[i][n] == 't')
            F->t = 1, F->v = 1;
          else if (argv[i][n] == 'T')
            F->t = 1;
          else if (argv[i][n] == 'v')
            F->v = 1;
          else
            result = 0, printf("cat: invalid option -- '%c'\n", argv[i][n]),
            printf("Try 'cat --help' for more information.\n");
        }
      }
    }
  }
  return result;
}

void stdio() {
  int ch;
  while ((ch = getchar()) != EOF) {  // work until ^Z (Ctrl+Z)
    putchar(ch);
  }
}

void printSruct(struct flags *F) {
  printf("struct F:\n");
  printf(" b = %d\n e = %d\n n = %d\n s = %d\n t = %d\n v = %d\n", F->b, F->e,
         F->n, F->s, F->t, F->v);
}

void printArgv(int *argc, char **argv) {
  // test init param
  for (int i = 0; i < *argc; i++) {
    printf("argc = %d argv = %s\n", i, argv[i]);
  }
}
