#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <pwd.h>
#include <signal.h>
#include <fcntl.h>
#include "SpecFuncs.h"
#include "HelperFuncs.h"
#define MAX_LINE 80
pid_t pid;

//Function to Kill Child Process
void killprocess()
{
  kill(pid, SIGTERM);
}
int main(void)
{
  //get current directory
  struct passwd *pw = getpwuid(getuid());
  char *homedir = pw->pw_dir;
  char inargs[MAX_LINE/2 +1];
  int i;
  //handle contolc
  struct sigaction act;
  act.sa_handler = killprocess;
  sigaction(SIGINT, &act, NULL);
  //empty inargs 
  for(i=0;i<MAX_LINE/2 +1;i++) inargs[i]=' ';
  int fd[2];
  int status;
  while (true){
    //open pipe
    if(pipe(fd)== -1){
      fprintf(stderr,"pipe failed");
      return 1;
    }
    //fork process
    pid = fork();
    if(pid>0){ //Parent Process
      clearArray(inargs);
      //while the inputs empty get the input
      while(inargs[0]==0){
        getInput(inargs);
      }
      //write the command to history
      writeToHistory(inargs,homedir);
      int ampersand = ampersandLast(inargs);
      //if there is an ampersand
      if(ampersand !=0){
        inargs[ampersand] = ' ';
        if (searchAndExecuteSpecFunc(inargs)==0){
          //write don't wait
          writeToPipe(fd,inargs,MAX_LINE/2 +1);
        }
      }
      //if there is not an ampersand
      else{
        if (searchAndExecuteSpecFunc(inargs)==0){
          //write and wait
          writeToPipe(fd,inargs,MAX_LINE/2 +1);
          wait(NULL);
        }
      }
    }
    if(pid==0){ //Child Process
      clearArray(inargs);
      //read from pipe
      readFromPipe(fd,inargs,MAX_LINE/2 +1);
      //Strip away the command
      char* command = strtok(inargs, " ");
      char* args[argcount(inargs)];
      memset(args, 0, sizeof args);
      //write the arguments
      writeargs(args,inargs);
      //execute the comnand
      execvp(command,args);
      //command failed continue
      printf("Command not found:  %s\n", command);
      return 0;
    }
  }
  return 0;
}



