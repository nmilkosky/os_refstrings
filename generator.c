/* CSC 345 Reference String Project
   Part 1
   Nate Milkosky */
#include <stdio.h>
#include <stdlib.h>
/* generates a reference string (inside list) of length "length", made out of pages from 
   the pages array, with positions from the setsPosition array. */
void  generate_spatial(int *list, int length, int *setsPosition, int spLength, 
		       int* pages, int pLength)
{	
	int counter = 0;
	// Loop until we fill the array
	while( counter < length )
	{
		int setStart = rand() % (spLength - 1);
		// the length of the subset will assist with the randomizer
		int subsetLength = setsPosition[setStart+1] -
				   setsPosition[setStart];
		
		// How many values that will be obtained from a set of pages
		int sectLength = (rand() % 15) + 5;
		// Counter to reset once we reach an the length of this section
		int sectCounter = 0;
		//Loop until we complete our time in a subsection (or when we finish)
		while( sectCounter < sectLength && counter < length )
		{
		    	//Assign the part in the string to a random page within the subset
			list[counter] = pages[sectCounter 
						+ setsPosition[setStart]];
			sectCounter++;
			counter++;
		}
		//printf("%d|%d\n", setsPosition[setStart], subLength);		

	}	
}

// generates a reference with temporal locality using an array passed to the function
// that will be modified (list), the length of the string (length), the page numbers that are the boundaries of processes (setsPosition), the length of the set position (spLength), and the page list and its length. 
void  generate_temporal(int *list, int length, int *setsPosition, int spLength, 
			int *pages, int pLength)
{
	int counter = 0;
    
    	while( counter < length )
    	{
		int setStart = 	rand() % (spLength - 1);	
		int sectCounter = 0;
		int sectLength = (rand() % 12)+ 3;	
		int tempNums = (rand() % 2) + 1;
		int temporalPageNum = (rand() % (setsPosition[setStart+1] - setsPosition[setStart] -(tempNums- 1)));	
		while( sectCounter < sectLength && counter < length )
		{
			int tempLoc = sectCounter % tempNums + temporalPageNum;
			list[counter] = pages[tempLoc + setsPosition[setStart]];
			sectCounter++;
			counter++;		   	
		}
	}
}

void generate_combo(int *list, int length, int *setsPosition, int spLength, 
	int *pages, int pLength)
{
	int counter = 0;
	while( counter < length )
	{
	
		int chooser = rand() % 2;
		int sectLength;
		int *listPtr = list + counter;
	
		if( chooser == 0 )
		{
			sectLength = (rand() % 15) + 5;  
			if(sectLength >= length)
			sectLength = length;
			generate_spatial(listPtr, sectLength, setsPosition, spLength, pages,
					 pLength);
			counter += sectLength;
		}

		if( chooser == 1 ) 
		{
		    	sectLength = (rand() % 12) + 3; 
			if(sectLength >= length)
			    sectLength = length;
			generate_temporal(listPtr, sectLength, setsPosition, spLength, pages,
					 pLength);
			counter += sectLength;
		}
	}

}

void generate_pr(int *list, int length, int *pages, int pLength)
{
	int counter = 0;
	while ( counter < length )
	{
		list[counter] = pages[rand() % pLength]; 	
	}
}
// main function
int main ( int argc, char *argv[])
{
	int pageList[100];
	int i;
	for (i = 0; i < 100; i++)
	{
		pageList[i] = i;
	}
	int pageSeparators[10] = {0, 7, 15, 21, 26, 44, 50, 64, 81, 100};
	srand(time(0));
	int refLen = atoi(argv[1]);
	int rString[refLen];
	generate_combo(rString, refLen, pageSeparators, 10, pageList, 100);
	for (i = 0; i < refLen; i++)
	{
	    printf("%d ", *(rString+i));
	}
	printf("\n");
	return 0;
}
