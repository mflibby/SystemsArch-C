//Program Created by Cameron Wheelock                                                             
//University of Southern Maine                                                                    
//Spring 2021 


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char* argv[]){

  int fd, sz, lc;
  int tlc = 0;
  char c[1024];
  
  if(argc < 2){
    printf("No files specified.\n");
    exit(1);
  }
  
  for(int i = 1; i<argc; i++){ //main loop for each argument 

    lc = 0;
    fd = open(argv[i], O_RDONLY); 

    if(fd < 0) { exit(1); }

    while((sz = read(fd, c, 1024)) != 0){
      for(int j = 0; j<sz; j++){
	if(c[j] == '\n') { lc++; };
      }
    }
    printf("%d\t%s\n",lc,argv[i]);
    tlc += lc;
  }
  if(argc > 2){
    printf("%d\tTotal\n", tlc);
    return tlc;
  }else{
    return lc;
  }
  
  close(fd);
};
