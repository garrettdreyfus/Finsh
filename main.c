#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <pwd.h>
#include "SpecFuncs.h"
#include "HelperFuncs.h"


int main(void)
{
  pid_t pid;
  struct passwd *pw = getpwuid(getuid());
  char *homedir = pw->pw_dir;
  char inargs[MAX_LINE/2 +1];
  int i;
  for(i=0;i<MAX_LINE/2 +1;i++) inargs[i]=' ';
  int should_run = 1;
  int fd[2];
  while (should_run){
    if(pipe(fd)== -1){
      fprintf(stderr,"pipe failed");
      return 1;
    }
    pid = fork();
    if(pid>0){
      getInput(inargs);
      writeToHistory(inargs,homedir);
      if (searchAndExecuteSpecFunc(inargs)==0){
        writeToPipe(fd,inargs,MAX_LINE/2 +1);
        wait(NULL);
      }
      clearArray(inargs);
    }
    if(pid==0){
      readFromPipe(fd,inargs,MAX_LINE/2 +1);
      char* command = strtok(inargs, " ");
      char* args[argcount(inargs)];
      writeargs(args,inargs);
      execvp(command,args);
      printf("Command not found:  %s\n", command);
      return 0;
    }
  }
  return 0;
}



