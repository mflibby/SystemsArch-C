/* execute.c - code used by small shell to execute commands */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <dirent.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pwd.h>

int cd ( char* pth );
int isin ( int num , int array[] );
void append ( int array[] , int new_member );
int bgjobs = 0;
int bgpids[10]  = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};


int execute ( char* argv[] )
/*
 * purpose: run a program passing it arguments
 * returns: status returned via wait, or -1 on error
 *  errors: -1 on fork() or wait() errors
 */
{
  int	pid ;
  int	child_info = -1;
  int bg = 0;
  int argc = 1;
  if ( argv[0] == NULL )		/* nothing succeeds	*/
    return 0;

  while(argv[argc] != NULL){
    argc++;
  }

  if (argv [ 1 ] != NULL & strcmp( argv [ argc - 1 ] , "&" ) == 0 ){
    bg = 1;
    argv[ argc - 1 ] = NULL;
  }
  
  if ( strcmp ( argv[0] , "exit" ) == 0 ) {/* Special Case: Exit [value]*/

    printf ( "Exiting with status %d\n" , ( argv [ 1 ] == NULL ) ? 0 : atoi ( argv [ 1 ] ) );
    exit ( ( argv[1] == NULL ) ? 0 : atoi ( argv [ 1 ] ) );

  } 
  else if ( strcmp ( argv[0] , "cd" ) == 0 ) { /* Special Case: cd [dir]*/

    cd(argv[1]);

  } 
  //GENERAL CASE
  else if ( (pid = fork())  == -1 ) /* Create fork, perror if it fails*/
    perror("fork");
  else if ( pid == 0 ){ 
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
    execvp(argv[0], argv);
    perror("cannot execute command");
    exit(1);
  }
  else {
    
    if ( waitpid ( -1 , &child_info , bg ? WNOHANG : 0 ) == -1 )
      perror("wait");
  }
  
  int temp;
  if ((temp = isin ( pid , bgpids )) != -1){
    bgjobs--;
    printf("[%d]+ Complete %d\n", temp, bgpids[temp]);
    bgpids[temp]=-1;
  }

  if ( bg ) {
    bgjobs++;
    printf("[%d] %d\n" , bgjobs ,pid);
    append(bgpids, pid);
  }
  return child_info;
}


int cd ( char* pth ){
  char path[200];
  /* Handle base case (cd to user home), i.e. pth==NULL*/
  if (pth == NULL){
    char* home;
    home = getpwuid( getuid ( ) ) -> pw_dir ;
    chdir( home );

  } else {
  strcpy(path,pth);
  /* identify if path is absolute or relative */
  if(pth[0] != '/') {//if it is relative, find cwd and use it for chdir
      char cwd[200];
      getcwd ( cwd , sizeof ( cwd ) );
      strcat ( cwd , "/" );
      strcat ( cwd , path );
      chdir ( cwd );
  
  } else {//if it is absolute, go there
  
      chdir(pth);
  }
  }
  return 0;
}


int isin(int num, int array[]){

  for ( int i = 0 ; i < 10 ; i++ ){
    if ( array [ i ] == num ){
      return i;
    }
  }
  return -1;
}

void append ( int array[] , int new_member ){

  for ( int i = 0 ; i < 10 ; i++ ){

    if (array[i] == -1){
      array[i] = new_member;
      return;
    }

  }

}