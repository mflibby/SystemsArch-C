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
        pthread_t t[ argc - 1 ];
        int* ret[ argc - 1 ];

        
        for ( int i = 1 ; i < argc ; i++ ) {

                pthread_create ( (&t[i]) , NULL , lc1 , (void *) argv [ i ] );

        }

        int filesProcessed = 0;


        for ( int i = 1 ; i < argc ; i++ ) {

                pthread_join ( (t[i]), ( void** ) &( ret [ i ] ) );
                printf ( "%d\t%s\n" , *(ret [ i ]) , argv [ i ] );
                count += *(ret [ i ]);

        }

        printf("%d\tTotal\n", count);

}
        


void *lc1(void *file){
        char *fileName = ( char* ) file;
        int fd , bite;
        char c[ 1024 ];
        int *lc = malloc ( sizeof ( int ) );
        *lc = 0;
        
        fd = open ( fileName , O_RDONLY );

        while ( ( bite = read ( fd , c , 1024 ) ) != 0 ) {
                for ( int i = 0 ; i < bite ; i++ ) {
                        if( c [ i ] == '\n' ){
                                ( *lc ) ++;
                        }
                }
        }
        close(fd);
        return lc;
}

