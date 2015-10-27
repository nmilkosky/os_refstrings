/* CSC345 Reference String Project
   Nate Milkosky
   Part 1 Driver*/
#include <stdio.h>
#include <stdlib.h>
#include "generator.h"

int main ( int argc, char *argv[])
{
	if(argc != 5) //incorrect args
	{
		printf("Usage: %s <string length> <# of processes> <# of pages> <type>\n" , argv[0]);
		printf("<type> should either be 1, 2, 3, or 4.\n");
		printf("1 chooses spatial locality. 2 chooses temporal locality.\n");
		printf("3 is a combination of both. 4 is a psuedorandom string. \n");
	}
	else //correct number of args
	{
		int pageListLen = atoi(argv[3]); //get the number of pages
		int pageList[pageListLen]; // init the list of pages
		int i; //init iterator
		for (i = 0; i < pageListLen; i++)
		{ //make the page list. each element will be the same as its index.
			pageList[i] = i;
		}//needed for the functions as the functions do not know whether the pagelist is in order
		srand(time(0)); //seed the random number generator
		int numProc = atoi(argv[2]);//number of processes is how many subsections we will have.
		int procs[numProc+1]; //make the processes position array
		procs[0] = 0; //make initial element
		for (i = 1; i < numProc; i++)
		{	//for each process, vary its number of pages slightly 
			procs[i] = (pageListLen/numProc) + (rand() % 4) - 2  + procs[i-1]; 	
		}
		procs[numProc] = pageListLen; //final item in process page bounds is the last page			
		int refLen = atoi(argv[1]);
		int rString[refLen]; //initialize the reference string
		int choice = atoi(argv[4]);
		switch(choice)
		{
			case 1:
			  generate_spatial(rString, refLen, procs, numProc, pageList, pageListLen);
			  break;
			case 2: 		  	
			  generate_temporal(rString, refLen, procs, numProc, pageList, pageListLen);	
			  break;
			case 3:
			  generate_combo(rString, refLen, procs, numProc, pageList, pageListLen);
			  break;
			case 4: 		  	
			  generate_pr(rString, refLen, pageList, pageListLen);
			  break;
			default: 
			  generate_pr(rString, refLen, pageList, pageListLen);
			  break;
		}
		for (i = 0; i < refLen; i++)
		{
		    printf("%d ", *(rString+i)); //print the string
		}
		printf("\n");
	}
	return 0;
}
