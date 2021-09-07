#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

/*HEADER*/
static long size(int fd);


/*GLOBALS*/
#define EOL '\n'


/*MAIN*/
int main ( int argc , char* argv[] ) {

        /*INITIALIZATION*/
        if ( argc < 2 ){// args?
                printf("No file input, exiting...");
                exit(0);
        }
        
        int fd;
        long fSize;
        long lc; 
        long total = 0;
        for ( int fc = 1 ; fc < argc ; fc++) {
                
                if ( ( fd = open ( argv [ fc ] , O_RDONLY ) ) == -1 ) {//file opens?
                        fprintf ( stderr , "Failed to open file %s" , argv [ fc ] );
                        exit(1);
                }

                fSize = size ( fd );//get file size for buffer calloc

                char* buff;
                buff = calloc ( 1, fSize + 1 );//give buffer the size of input file

                if ( ( read ( fd , buff , fSize ) ) == -1 ) {
                        close ( fd );
                        free ( buff );
                        fprintf ( stderr , "A critical failure occured when reading in the file %s.\n" , argv[fc] );
                        exit ( 1 );
                }
                close ( fd );

                /*GENERAL*/
                lc = 0;
                for ( int i = 0; i < fSize; i++ ) //count EOL
                        if ( buff[i] == EOL )
                                lc++;

                //fprintf ( stdout , "The file %s has %ld lines.\n" , argv [ fc ] , lc );
                //printf ( "HELLO!\n" );
                /*CLEANUP*/
                printf ( "%ld\t%s\n" , lc , argv [ fc ] );

                free ( buff );
                total += lc;
        }
        if ( argc > 2 ) { 
                printf ( "%ld\n" , total );
        }
        return 0;
}
//------------------EOM-------------------

/*AUXILLIARY METHODS*/

static long size ( int fd ) {

  long fsize;

  
  fsize = lseek( fd , 0L , SEEK_END );
  lseek( fd , 0L , SEEK_SET );
  
  return fsize;
  
}