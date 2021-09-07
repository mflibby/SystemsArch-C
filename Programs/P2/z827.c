#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <getopt.h>

#define VERSION "1.0.0"

//option elements
static bool success_flag = false;
static bool verbose_flag = false;
static bool keep_flag = false;
static bool brief_flag = false;
static struct option long_options[] = {
    {"verbose", no_argument, 0, 'v'},
    {"brief",   no_argument, 0, 'b'},
    {"keep",    no_argument, 0, 'k'},
    {"version", no_argument, 0, 'V'}
};

int main(int argc, char **argv){
	FILE *input;

  	//HANDLE OPTIONS&ARGS
  	int c;
  	int long_index = 0;
  
  	while ((c = getopt_long(argc,argv,"vbkV::", long_options, &long_index)) != -1){
    	switch(c){
      		case 'v':
        		verbose_flag = true;
        		break;
      		case 'b':
        		brief_flag = true;
	    		verbose_flag = false;
	    		break;
      		case 'k':
        		keep_flag = true;
	    		break;
      		case 'V':
	    		printf("z827 %s\n", VERSION);
	    		exit(0);
      		case '?':
				fprintf(stderr, "Unknown option character %c\n", optopt);
				exit(1);
      		default:
        		continue;
		}
	}
  	//NON-OPTION ARGS
	if (optind < argc){
    	if ( ( input = fopen ( argv[optind], "rb" ) ) == NULL){
      		fprintf(stderr, "The file \"%s\" could not be found.\n", argv[optind]);
      		exit(1);
    	}
    	if (optind < argc-1){
      		fprintf(stderr, "This program currently cannot compress multiple files in one go, ignoring additional args after \"%s\"\n", argv[optind]);
    	}
  	} else {
    	fprintf(stderr, "Need a file to (de)compress!\n");
    	exit(1);
  	}
  
  	//END OPTIONS&ARGS HANDLING
	
	//SET UP (DE)COMPRESSION ELEMENTS
	unsigned int buff = 0;
  	unsigned char curr;
  	int bitc = 0;
  	int size = 0;
  	int procBuff = 0;
  	char fn[strlen(argv[optind])];
  	
  	//FIND FILE SIZE
    fseek(input,0L,SEEK_END);
    size = ftell(input);
    rewind(input);
  
  	strcpy(fn, argv[optind]);
  	if (strstr(fn,"z827") == NULL){
  		//COMPRESS
  		
    	printf(brief_flag ? "" : "Compressing file...\n");
    	
    	printf(verbose_flag ? "Original file size: %d bytes\n" : "", size);
  
    	fread(&curr, 1, 1, input);
    	buff |= curr;
    	bitc += 8;
    	procBuff ++;
    	FILE *output = fopen(strcat(argv[optind],".z827"), "w+b");

    	fwrite(&size, 4, 1, output);
    
    	while ( fread(&curr, 1, 1, input) == 1){
      		procBuff++;
      		buff |= curr << bitc-1;
      		bitc += 7;

      		while (bitc > 8){
				fwrite(&buff, 1, bitc/8, output);
				buff >>= 8*(bitc/8);
				bitc -= 8*(bitc/8);

      		}
      		
    	}
		//CLEANUP
    	fwrite(&buff, 1, 1,output);
    	fclose(input);
    	fclose(output);
    	if (procBuff == size){
      		printf(verbose_flag ? "Compression Successful!\nCleaning up...\n" : "");
      		success_flag = true;
      		if(!keep_flag)
				unlink(fn);
     	}

  	} else {
  		//DECOMPRESS
  		
    	printf(brief_flag ? "" : "Decompressing file...\n");
    	
    	printf(verbose_flag ? "Original file size: %d bytes\n" : "", size);
  

    	//ignore first 4 bits of file, which is compression info
    	fseek(input, 4, SEEK_SET);
    	procBuff+=4;
    	//remove .z827 extension from input file string copy
    	fn[strlen(fn)-5]='\0';
    
    	FILE *output = fopen(fn, "w+b");
    
    	while ( fread(&curr, 1, 1, input) == 1){
      		buff |= curr << bitc; //append next char to the buffer
      		bitc += 8;//add how many bits were just added to the buffer
      		procBuff ++;

      		//there are more than 7 bits left in the buffer, keep writing to the file until this is not so:
			while(bitc>7){
				curr = buff & 0x7f;
				fwrite(&curr, 1, 1, output);
				buff >>= 7;
				bitc -= 7;
				
	  		}
    	
    	}
    	//CLEANUP
    	fclose(input);
    	fclose(output);
    	if (procBuff == size){
    		printf(verbose_flag ? "Decompression Successful!\nCleaning up...\n" : "");
      		success_flag = true;
      		if (!keep_flag)
				unlink(argv[optind]);
    	}
	}
  	
	if (!brief_flag);
  		printf(success_flag ? "Done!\n" : "Compression Failed, make sure input file is ASCII compliant.\n");
  	return 0;
}