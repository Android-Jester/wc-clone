#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define MAX_LINE 128
#define RED     "\033[31m"
#define RESET   "\033[0m"

int counter(FILE *file, char *option) {
  int count = 0;
  if (strcmp(option, "-c") == 0) {
    while (fgetc(file) != EOF) {
      count++;
    }
  } else if (strcmp(option, "-l") == 0) {
    char line[MAX_LINE];
    while (fgets(line, MAX_LINE, file) != NULL) {
      count++;
    }
  } else if (strcmp(option, "-w") == 0) {
    char ch;
    int word = 0;

    while ((ch = fgetc(file)) != EOF) {
      if (ch == ' ' || ch == '\n') {
        word = 0;
      } else {
        if (word == 0) {
          count++;
          word = 1;
        }
      }
    }
  } else {
    if(option[0] == '-') {
      fprintf(stderr, RED "Option %s Not available" RESET, option);
    }
    else {
      fprintf(stderr, RED "Wrong input: %s" RESET, option);
    }
    exit(-1);

  }

  rewind(file);
  return count;
}

void unspecifiedCounts(FILE *file, char *fileName) {
  printf("%d ", counter(file, "-l"));
  printf("%d ", counter(file, "-c"));
  printf("%d ", counter(file, "-w"));
  printf("%s\n", fileName);
}

void readFromStdin(int argc, char *arg) {
  if (argc == 2) {
    counter(stdin, arg);
  } else {
    unspecifiedCounts(stdin, "Standard Input");
  }
}

void readFromFile(int argc, char **argv) {
  FILE *file;
  if (argc == 2 && (access(argv[1], F_OK) == 0)) {
    file = fopen(argv[1], "r");
    unspecifiedCounts(file, argv[1]);
  } else if (argc == 3) {
    if (access(argv[2], F_OK == -1 && argv[2][0] == '-')) {
      fprintf(stderr, RED "Wrong position of option" RESET);
      exit(-1);
    } else {
      file = fopen(argv[2], "r");
      printf("%d", counter(file, argv[1]));
    }
  } else {
    printf("[***] Unable to count\n");
    exit(EXIT_FAILURE);
  }
  fclose(file);
}

int main(int argc, char **argv) {
  if (isatty(STDIN_FILENO) == 0) {
    readFromStdin(argc, argv[1]);
  } else if ((argc == 1 || strcmp(argv[1], "-h")) == 0) {
    printf("[*] Usage: wcc [OPTION] [FILE]\n");
    printf("wc-clone is a cli tool to check the count of lines, characters and "
           "words within a file or stdin when using the pipe\n");
    printf("Options:\n");
    printf("[->] -l: Line Count\n");
    printf("[->] -w: Word Count\n");
    printf("[->] -c: character Count\n");
  } else {
    readFromFile(argc, argv);
  }
  return EXIT_SUCCESS;
}
