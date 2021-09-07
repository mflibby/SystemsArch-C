#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

/*HEADER*/



/*GLOBALS*/
int total = 0;


/*MAIN*/
int main ( int argc , char* argv[] ) {

        /*INITIALIZATION*/
        if ( argc < 2 ) {// args?
                printf ( "No files input, exiting..." );
                exit ( 0 );
        }

        int pipefd[ 2 ];
        pid_t cpid[ argc - 1 ];


        if ( pipe ( pipefd ) == -1 ) {
                perror ( "pipe" );
                exit ( 1 );
        }


        for ( int i = 0 ; i < argc - 1 ; i++ ) {

                if ( ( cpid [ i ] = fork ( ) ) == -1 ) { //create fork
                        perror ( "fork" );
                        exit ( 1 );
                }

                if ( cpid [ i ] == 0 ) {    /* Child process control */
                        dup2 ( pipefd [ 1 ] , 1 );
                        close ( pipefd [ 0 ] );
                        close ( pipefd [ 1 ] );

                        if ( execlp ( "lc1" , "lc1" , argv [ i + 1 ] , NULL ) == -1 )
                                perror("execution failed");
                        
                        exit(EXIT_SUCCESS);

                }  
        }

        /* Parent Process control */
        int children = 0;
        while ( children < ( argc-1 ) ) {
                wait ( 0 );
                children++;
        }

        char buf[BUFSIZ];
        int len;
        len += read(pipefd[0], &buf[len], BUFSIZ);

  
        int count = 0;
  
        close(pipefd[0]);
        close(pipefd[1]);
        
        if ( ( write ( 1 , buf , len ) ) == -1 ) {
                fprintf ( stderr , "Write error" );
                exit ( 1 );
        }

        fflush ( stdout );

        return 0;
}