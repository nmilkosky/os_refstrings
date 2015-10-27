/* CSC345 Reference String project
   Part 3 Driver
   Nate Milkosky */
#include <stdio.h>
#include <stdlib.h>
#include "generator.h"
#include "simulator.h"

int main ( int argc, char *argv[])
{
	if(argc != 6) //incorrect args
        {
                printf("Usage: %s <string length> <# of processes> <# of pages> <type> <frames available>\n" , argv[0]);
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
                {       //for each process, vary its number of pages slightly  
                        procs[i] = (pageListLen/numProc) + (rand() % 4) - 2  + procs[i-1]; 
                }
                procs[numProc] = pageListLen; //final item in process page bounds is the last page
		int refLen = atoi(argv[1]);
                int rString[refLen]; //initialize the reference string
                int choice = atoi(argv[4]);     
                switch(choice)
                {
                        case 1:
                          generate_spatial(rString, refLen, procs, numProc+1, pageList, pageListLen);
                          break;
                        case 2:
                          generate_temporal(rString, refLen, procs, numProc+1, pageList, pageListLen);
                          break;
                        case 3:
                          generate_combo(rString, refLen, procs, numProc+1, pageList, pageListLen);
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
		printf("\n\n");
		int framesSize = atoi(argv[5]);
		int pageFaultsFifo = sim_fifo(rString, refLen, framesSize);
		int pageFaultstLRU = sim_tLRU(rString, refLen, framesSize);
		int pageFaultssecC = sim_secC(rString, refLen, framesSize);
		printf("FIFO: %d \n", pageFaultsFifo);
		printf("tLRU: %d \n", pageFaultstLRU);
		printf("secC: %d \n", pageFaultssecC);
	}
	return 0;
}
