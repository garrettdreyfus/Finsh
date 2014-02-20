#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <pwd.h>
#include <wordexp.h>
#define NUMBER_OF_FUNCS 2
typedef int bool;
#define true 1
#define false 0
typedef void (*DoRunTimeChecks)();
struct ShellFunc
{
  char* keyword;
  DoRunTimeChecks func;

};
void cdFunc(char * args[]);
int searchStructs(char* key,struct ShellFunc* funcs);
void append(char* s,  char c);
struct ShellFunc shell_funcs[NUMBER_OF_FUNCS];
