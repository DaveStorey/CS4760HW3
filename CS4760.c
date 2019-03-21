//David Storey
//CS4760
//Project 3

#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<getopt.h>
#include "help.h"
#include "fileSpec.h"

int main(int argc, char * argv[]){      
	int opt, result, x = 0;
	char * outfile;
	char * infile;
	//Assigning default case names.
	if (argc == 1){
		infile = "input.dat";
		outfile = "output.dat";
	}
	if((opt = getopt(argc, argv, "-hio")) != -1){
		do{
			x++;
			switch(opt){
			case 'h':
				help();
				break;
			case 'i':
				//Check to ensure order of options.
				if(x > 1){
					printf("Error, -i must be the first option if present.\n");
					return(0);
				}
				//If -i and -o options given, assigning correct user input to
				//output file string.
				if(argc == 5){
					outfile = argv[5];
				}
				//Assigning default if -o option not used.
				else
					outfile = "output.dat";
				result = fileSpec(argv[x+1], outfile);
				break;
			case 'o':
				//Case used when user selects -o but not -i, assigns default
				//input file option.
				if(argc == 3){
					infile = "input.dat";
					result = fileSpec(infile, argv[x+1]);
				}		
				break;
			//Case handles last run through of getopt.
			case 1:
				break;
			}
		} while((opt = getopt(argc, argv, "-hio")) != -1);
	}
	//Else statement handles case when user selects no options.
	else
		fileSpec(infile, outfile);
	return 0; 
}
