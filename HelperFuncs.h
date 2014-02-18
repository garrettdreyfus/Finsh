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
//Changes string to array of strings. Always appends NULL
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

// Writes all Calls to History
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

//counts the number of args to expect
int argcount(char *str)
{
  int num=0;
  bool flag=false;
  str[strlen(str)]=' ';
  for(int i =0; i < strlen(str); i++)
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

//Prompt Function
void prompt()
{
  printf( RED ">  " RESET);
}
void writeToPipe(int pipe[],char *content, int buffer)
{
  close(pipe[READ_END]);
  write(pipe[WRITE_END], content,buffer);
  close(pipe[WRITE_END]);
}
void readFromPipe(int pipe[],char *content, int buffer)
{
  close(pipe[WRITE_END]);
  read(pipe[READ_END],content,buffer);
  close(pipe[READ_END]);
}
void getInput(char *inargs)
{
  rl_bind_key('\t', rl_complete);
  char *inputstring = readline (RED ">" RESET);
  if(inputstring[0] != 0){strcpy(inargs,inputstring);};
  //if(argcount(inputstring)==0) getInput(inputstring);
}
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

