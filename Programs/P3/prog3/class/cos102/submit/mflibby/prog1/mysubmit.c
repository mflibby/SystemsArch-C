#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <pwd.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>

int requestString(char* request, char* inputBuffer);
int listfiles(char* files, char* dir);
void printFileProperties(struct stat stats);
int showFiles(char* dir, struct stat input);
int copyFile(char* fileName, char* targetDir);

#define CLASS_BIT 1
#define NAME_BIT 2
#define FILE_BIT 4
#define CBUFF_SIZE 8
char classBuf[CBUFF_SIZE];
char dirBuf[500];
char nameBuf[500];
char fileBuf[500];
char* cpBuf;
char* buff;
char dummyBuf[100];
int sByte = 0;

int main(int argc, char** argv){
  struct passwd userCredin;
  struct passwd* userCredout;
  struct stat input;
  struct dirent ***namelist;
  uid_t user = getuid();

  
  //collect user info:
  getpwuid_r(user, &userCredin, dummyBuf, 100, &userCredout);

  //ensure class folder exists, query again if not:
  strcat(dirBuf, "class/");
  while(!(sByte & CLASS_BIT)){
    requestString("Please enter class number (e.g.\"COS350\"):", classBuf);
    strcat(dirBuf, classBuf);
    if (stat(dirBuf, &input) == 0){
      sByte |= CLASS_BIT;
      //break;
    } else {
      dirBuf[8] = 0;
    }
    //  printf("Full dir: %s\n", dirBuf );
  }

  strcat(dirBuf, "/submit");

  //verify that submit exists
  if(stat(dirBuf, &input) != 0){
    printf("This class is not currently accepting submissions.\n");
    exit(1);
  }
  strcat(dirBuf, "/");

  //get username
  strcat(dirBuf, userCredin.pw_name);
  //check if user folder exists in specified dir, if not create it
  
  if(stat(dirBuf, &input) != 0){
    //create user director inside specified submit folder
    mkdir(dirBuf, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  }
  
  strcat(dirBuf, "/");
  //query for assignment name, create directory if it doesn't already exist
  while(!(sByte & NAME_BIT)){
    requestString("Please enter assignment name (e.g.\"prog1\"):", nameBuf);
    strcat(dirBuf, nameBuf);
    if (stat(dirBuf, &input) == 0){
      sByte |= NAME_BIT;
    } else {
      mkdir(dirBuf, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
      sByte |= NAME_BIT;
    }
    printf("Full dir: %s\n", dirBuf );
  }

  
  showFiles(".", input);

  //get list of files to submit
  requestString("Which files do you want to submit (use spaces to separate files): ", fileBuf);
  //allow for submitting *
  if (fileBuf[0] == '*'){
    fileBuf[0] = 0;
    listfiles(fileBuf, ".");
  }
  printf("Files: %s\n", fileBuf);
  cpBuf = (char*) malloc(100);
  cpBuf = strtok(fileBuf, " ");
  while(cpBuf != NULL){
    printf("Copying %s\n", cpBuf);
    copyFile(cpBuf, dirBuf);
    cpBuf = strtok(NULL, " ");
  }
  printf("\n");
  
  //show files in submission directory
  showFiles(dirBuf, input);
}


int requestString(char* request, char* inputBuffer){  
  int c, i = 0;
  printf("%s", request);
  while((c = getchar()) != EOF & c != '\n'){
    inputBuffer[i++] = c;
  }
  classBuf[i] = 0;
  putchar('\n');
}


int copyFile(char* fileName, char* targetDir){
  char tarTemp[500] = "";
  strcat(tarTemp, targetDir);
  strcat(tarTemp, "/");
  strcat(tarTemp, fileName);
  FILE* source;
  source = fopen(fileName, "r");
  FILE* target;
  target = fopen(tarTemp, "w+");
  char buff;
  while ( fread(&buff, 1, 1, source)!= 0){
    fwrite(&buff, 1, 1, target);
  }
  fclose(source);
  fclose(target);
}

int listfiles(char* files, char* dir){

  struct dirent **namelist;
  int n;
  int j = 0;
  if ((n = scandir(dir, &namelist, NULL, alphasort)) == -1){
    fprintf(stderr, "Failed to query directory '%s' for file list\n", dir);
    return(1);
  }
  while (j < n){
    if ((namelist[j]->d_name)[0]!='.'){
      strcat(files, namelist[j]->d_name);
      if (j != n-1){
	strcat(files, " ");
      }
    }
    j++;
  }
  printf("\n");
  
}

int showFiles(char* dir, struct stat input){
  struct dirent **namelist;
  struct tm dt;
  int n;
  int j = 0;
  if ((n = scandir(dir, &namelist, NULL, alphasort)) == -1){
    fprintf(stderr, "Failed to query directory '%s' for file list\n", dir);
    return(1);
  }
  printf("\n-File-\t\t-Size-\t-Last Mod-\n");
  while (j < n){
    if ((namelist[j]->d_name)[0]!='.'){
      printf("%-15s", namelist[j]->d_name);
      stat(namelist[j]->d_name, &input);
      dt = *(gmtime(&input.st_mtime));
      printf("%ld\t", input.st_size);
      printf("%d-%d-%d %d:%d:%d\n", dt.tm_mday, dt.tm_mon+1, dt.tm_year + 1900,dt.tm_hour, dt.tm_min, dt.tm_sec);

    }
    j++;
  }
  printf("\n");
}


