//Program Created by Cameron Wheelock
//University of Southern Maine
//Spring 2021

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>
#include <sys/types.h>
#include <fcntl.h>
void *lc1(void *);

void main(int argc, char* argv[]){

  
  int count = 0;
  pthread_t t1, t2, t3, t4, t5, t6, t7, t8, t9, t10;
  int *ret1, *ret2, *ret3, *ret4, *ret5;
  int *ret6, *ret7, *ret8, *ret9, *ret10;
  
  if(argc != 11){
    printf("Must use all 10 'f' files\n");
    exit(1);
  }
  
  pthread_create(&t1, NULL, lc1, (void *) argv[1]);
  pthread_create(&t2, NULL, lc1, argv[2]);
  pthread_create(&t3, NULL, lc1, argv[3]);
  pthread_create(&t4, NULL, lc1, argv[4]);
  pthread_create(&t5, NULL, lc1, argv[5]);
  pthread_create(&t6, NULL, lc1, argv[6]);
  pthread_create(&t7, NULL, lc1, argv[7]);
  pthread_create(&t8, NULL, lc1, argv[8]);
  pthread_create(&t9, NULL, lc1, argv[9]);
  pthread_create(&t10, NULL, lc1, argv[10]);

  int filesProcessed = 0;

 
  pthread_join(t1, (void**)&ret1);
  printf("%d\t%s\n", *ret1, argv[1]);
  count+=*ret1;
  
  pthread_join(t2, (void**)&ret2);
  printf("%d\t%s\n", *ret2, argv[2]);
  count+=*ret2;
  
  pthread_join(t3, (void**)&ret3);
  printf("%d\t%s\n", *ret3, argv[3]);
  count+=*ret3;
  
  pthread_join(t4, (void**)&ret4);
  printf("%d\t%s\n", *ret4, argv[4]);
  count+=*ret4;
  
  pthread_join(t5, (void**)&ret5);
  printf("%d\t%s\n", *ret5, argv[5]);
  count+=*ret5;
  
  pthread_join(t6, (void**)&ret6);
  printf("%d\t%s\n", *ret6, argv[6]);
  count+=*ret6;
  
  pthread_join(t7, (void**)&ret7);
  printf("%d\t%s\n", *ret7, argv[7]);
  count+=*ret7;
  
  pthread_join(t8, (void**)&ret8);
  printf("%d\t%s\n", *ret8, argv[8]);
  count+=*ret8;
  
  pthread_join(t9, (void**)&ret9);
  printf("%d\t%s\n", *ret9, argv[9]);
  count+=*ret9;
  
  pthread_join(t10, (void**)&ret10);
  printf("%d\t%s\n", *ret10, argv[10]);
  count+=*ret10;

  printf("%d\tTotal\n", count);

}
  
void *lc1(void *f){
  char *fileName = (char *) f;
  int fd, sz;
  char c[1024];
  int *myCount = malloc(sizeof(int));
  *myCount = 0;
  
  fd = open(fileName, O_RDONLY);

  while((sz = read(fd, c, 1024)) != 0){
    for(int i = 0; i<sz; i++){
      if(c[i] == '\n'){
	(*myCount)++;
      }
    }
  }
  close(fd);
  return myCount;
}

