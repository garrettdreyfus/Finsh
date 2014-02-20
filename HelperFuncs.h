#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <pwd.h>
#include <readline/readline.h>
#include <readline/history.h>
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"
#define MAX_LINE 80
#define READ_END 0
#define WRITE_END 1
void writeargs(char *arr[],char *str);
void writeToHistory(char *command, char *homedir);
int argcount(char *str);
char* removeFirstWord(char *str);
void prompt();
void writeargs(char *arr[],char *str);
void writeToHistory(char *command, char *homedir);
int argcount(char *str);
void prompt();
void writeToPipe(int pipe[],char *content, int buffer);
void readFromPipe(int pipe[],char *content, int buffer);
void getInput(char *inargs);
void clearArray(char *inarr);
void copyArr(char *arr1, char *arr2);
int searchAndExecuteSpecFunc(char *input);
int ampersandLast(char *str);


