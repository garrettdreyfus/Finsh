#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <pwd.h>
#include <wordexp.h>

typedef int bool;
#define true 1
#define false 0
typedef void (*DoRunTimeChecks)();
void append(char* s, char c);
void cdFunc(char * args[]);
char * slice(char* arr,int begin,int end);
struct ShellFunc
{
  char* keyword;
  DoRunTimeChecks func;

};

void cdFunc(char * args[]){
  char* path = args[1];
  wordexp_t exp_result;
  if(wordexp(path, &exp_result, 0)==0){
    chdir(exp_result.we_wordv[0]);
  }
  else {
    printf("Well Thats Not a directory");
  }
  wordfree(&exp_result);
}
void exitFunc()
{
  exit(0);
}
const int NUMBER_OF_FUNCS=2;
struct ShellFunc shell_funcs[NUMBER_OF_FUNCS] = {
  {"cd",cdFunc},
  {"exit",exitFunc},
};

int searchStructs(char* key,struct ShellFunc* funcs)
{
  int i;
  for(i=strlen(key);i>=0;i--) {
    if(i != ' '){
      key[i+3] = '\0';
      break;
    }
  }
  for(i=0;i<NUMBER_OF_FUNCS;i++){
    if(strcmp(key,funcs[i].keyword) ==0){
      return i;
    }
  }
  return -1;
}
void append(char* s,  char c)
{
        int len = strlen(s);
        s[len] = c;
        s[len+1] = '\0';
}
char * slice(char* arr,int begin,int end)
{
  char *newarr;
  int i;
  int counter;
  counter =0;
  for(i=begin;i<=end;i++){
    newarr[counter]=arr[i];
    counter ++;
  }
  return newarr;
}


