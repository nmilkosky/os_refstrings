/* CSC 345 Reference String Project
   Generator functions (for part 1)
   Nate Milkosky */
#include "generator.h"
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
	}	
}

// generates a reference with temporal locality using an array passed to the function
// that will be modified (list), the length of the string (length), the page numbers that are the boundaries of processes (setsPosition), the length of the set position (spLength), and the page list and its length. 
void  generate_temporal(int *list, int length, int *setsPosition, int spLength, 
			int *pages, int pLength)
{
	int counter = 0; //counter to keep track of where we are
    
    	while( counter < length ) //loop until we fill the array
    	{
		//randomly select a position from setsPosition (containing the page range for a process)
		int setStart = 	rand() % (spLength - 1);
		int sectCounter = 0; // counter that will determine when to stop using this locality
		int sectLength = (rand() % 12)+ 3; //how many page references this area  will use (3-15)
		int tempNums = (rand() % 2) + 1; // indicates whether we will reference 1 or 2 pages
		//here we choose a random page within the range of pages that a process contains
		//it's a random position from the beginning of the locality to (end - tempNums). 
		//we factor in tempNums so we don't bleed into another process's pages
		int temporalPageNum = (rand() % (setsPosition[setStart+1] - setsPosition[setStart] -(tempNums- 1)));	
		while( sectCounter < sectLength && counter < length ) //loop until we have completed the
		{ //section or until the reference string is complete
			//the location of the page that will be added to the ref string
			int tempLoc = sectCounter % tempNums + temporalPageNum; 
			list[counter] = pages[tempLoc + setsPosition[setStart]]; //add it to the string
			sectCounter++; //added an element from the section
			counter++; //added to ref string, so move to next element
		}
	}
}

//generates a combination of spatial and temporal locality. list is the list that will contain the ref
//string, length is the length of the reference string, setsPosition is the array that contains the rang
//es of the sets, spLength is the length of the setsPosition array, pages is the list of pages, and 
//pLength is the length of the pages
void generate_combo(int *list, int length, int *setsPosition, int spLength, int *pages, int pLength)
{
	int counter = 0; // keep track of where we are in the ref string
	while( counter < length ) //loop until we fill the array
	{
		int chooser = rand() % 2; //chooses spatial or temporal locality
		int sectLength; //this will keep track of how many entries from a type we will use
		
		//this pointer will keep track of what element in the array we are currently on.
		//it is useful as we can pass a subsection of the reference string to one of the 
		//functions, so that we don't have to specify where we are in the array as a parameter 
		//for the spatial or temporal functions
		int *listPtr = list + counter;
	
		if( chooser == 0 ) //if chooser is 0, we will do spatial
		{
			sectLength = (rand() % 10) + 5;  //make a section of length 5-15
			if(sectLength + counter >= length) //if the section would go over the bounds,
				sectLength = length - counter; //we have to make it smaller so no segfaults occur
			generate_spatial(listPtr, sectLength, setsPosition, spLength, pages,
					 pLength);
			counter += sectLength; //increase the counter.
		}

		if( chooser == 1 ) // chose temporal
		{
		    	sectLength = (rand() % 9) + 3; //make a section from length 3 - 12
			if(sectLength + counter >= length) //make sure section won't go out of bounds
			    sectLength = length - counter;
			generate_temporal(listPtr, sectLength, setsPosition, spLength, pages,
					 pLength);
			counter += sectLength; //increase the counter
		}
	}
}

void generate_pr(int *list, int length, int *pages, int pLength)
{
	int counter = 0; //counter to keep track of where we are
	while ( counter < length ) //make the string
	{
		list[counter] = pages[rand() % pLength]; //assign a part to a random page
		counter++; //move on
	}
}
