#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <pwd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "SpecFuncs.h"
#include "HelperFuncs.h"
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

//Takes a Char Array and Writes the words seperated by spaces into an array of strings
void writeargs(char *arr[],char *str)
{
  if(argcount(str)!= 0){
    char *words = strtok(str, " ");
    int counter=0;
    while (words != NULL)
    {
      arr[counter] = words;
      words = strtok (NULL, " ");
      counter++;
    }
    arr[counter]=NULL;
    int i; 
  }
  else{
    arr[0]=NULL;
  }
}

// Writes all calls to a history file titled ~/.finshistory
void writeToHistory(char *command, char *homedir)
{
  char home[strlen(homedir)+strlen("/.finshistory")];
  memset(home, 0, sizeof home);
  strcat(home, homedir);
  strcat(home, "/.finshistory");
  FILE *f = fopen(home, "a+");
  if (f == NULL)
  {
      printf("Error opening file!\n");
      exit(1);
  }
  fprintf(f,"%s",command);
  fclose(f);
}

//Counts the number of Arguments in a string
int argcount(char *str)
{
  int num=0;
  bool flag=false;
  str[strlen(str)]=' ';
  int i;
  for(i =0; i < strlen(str); i++)
  {
    if(str[i] != ' ' )
    {
      flag=true;
    }
    else if(flag && str[i]==' '){
      flag=false;
      num++;
   }
  }
  return num;
}

//Simple wrapper to write to pipe and close some ends
void writeToPipe(int pipe[],char *content, int buffer)
{
  close(pipe[READ_END]);
  write(pipe[WRITE_END], content,buffer);
  close(pipe[WRITE_END]);
}

//Simple wrapper to read from pipe and close some ends
void readFromPipe(int pipe[],char *content, int buffer)
{
  close(pipe[WRITE_END]);
  read(pipe[READ_END],content,buffer);
  close(pipe[READ_END]);
}

//Takes in a char array reads input with readline and copies it to the input
void getInput(char *inargs)
{
  clearArray(inargs);
  rl_bind_key('\t', rl_complete);
  static char *inputstring = (char *)NULL;
  inputstring= readline (RED ">" RESET);
  if(inputstring && *inputstring){
    add_history(inputstring);
    strcpy(inargs,inputstring);
    free (inputstring);
    inputstring = (char *)NULL;
  }
}

//Gets the index of the ampersand primarily used for background processes
int ampersandLast(char *str)
{
  int i;
  for(i=strlen(str)-1;i>0;i--)
  {
    if(str[i] == '&')
    {
        return i;
    }
  }
  return 0;
}

//Clears array by writing all contents to 0
void clearArray(char *inarr)
{
  memset(inarr, 0, sizeof strlen(inarr));
}
void copyArr(char *arr1, char *arr2)
{
  int i;
  for(i=0;i<strlen(arr2);i++)
  {
    arr2[i]=arr1[i];
  }
}

//Reads the command and checks it against its array of known special functions.
//i.e. those like cd which are not actual binaries and must be executed by the parent
//returns 0 if it is not a special function
int searchAndExecuteSpecFunc(char *input)
{
  char copy[strlen(input)];
  copyArr(input,copy);
  char* command = strtok(copy, " ");
  int index = searchStructs(command,shell_funcs);
  if(index !=-1){
    if (argcount(input)==1){
      char *fakeargs[2];
      fakeargs[0]= "cd";
      fakeargs[1]= "~";
      shell_funcs[index].func(fakeargs);
    }
    else{
      char* args[argcount(input)];
      writeargs(args,input);
      shell_funcs[index].func(args);
    }
    return 1;
  }
  return 0;
}

