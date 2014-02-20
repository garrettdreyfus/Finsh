#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <pwd.h>
#include <wordexp.h>
#include "SpecFuncs.h"
#include "HelperFuncs.h"
#define true 1
#define false 0

//Simple Function that uses wordexp to expand the given path and changes to that directory
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

//exits the program
void exitFunc()
{
  exit(0);
}

//List of known Special functions. Those that must be run by the parent
struct ShellFunc shell_funcs[NUMBER_OF_FUNCS] = {
  {"cd",cdFunc},
  {"exit",exitFunc},
};

//determines if the command is a special function
//returns the index of the SpecFunc
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

//apends char to end of string
void append(char* s,  char c)
{
        int len = strlen(s);
        s[len] = c;
        s[len+1] = '\0';
}



