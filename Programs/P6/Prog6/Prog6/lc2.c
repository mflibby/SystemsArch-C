//Program Created by Cameron Wheelock                                                              
//University of Southern Maine                                                                     
//Spring 2021 


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

void main(int argc, char* argv[]){

  int pipefd[2];
  int total = 0;
  pid_t pid;
  int len;
  char buf[BUFSIZ];
  int read_len=0;
  int children = 0;
  if(argc < 2){
    printf("No Files Specified: LC2\n");
    exit(1);
  }
  argv[0] = "lc1";
  if(pipe(pipefd) == -1){
    printf("Cannot get a pipe");
  }
  for(int i=1; i<argc; i++){
         
    if((pid = fork()) == -1){
	perror("fork failed");
      }
    char *lc1[2];
    lc1[0] = "lc1";
    lc1[1] = argv[i];    
    if(pid == 0){
      dup2(pipefd[1], 1);
      close(pipefd[0]);
      close(pipefd[1]);
      if((execvp(lc1[0],lc1)) == -1){
	perror("exec failed\n");
      }
    }
  }

  
  while(children < (argc-1)){
    wait(0);
    children++;
  }
  
  read_len += read(pipefd[0], &buf[read_len], BUFSIZ);
  //count+= atoi(strtok(&buf[oldPos], " "));
  
  int count = 0;
  
  close(pipefd[0]);
  close(pipefd[1]);

  children = 0;
  char *token;
  //while(children < (argc-1)){
    
  //  token = strtok(&buf[children*2], " ");
  //  count+=atoi(token);
  //  children--;
  //}
    
  if((write(1, buf, read_len)) == -1){
    printf("Write error");
    exit(1);
  }
  printf("%d\tTotal\n", count);
  fflush(stdout);
}
