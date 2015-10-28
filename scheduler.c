/* CSC 345 Reference String Project
   Part 4, Scheduler Simulator
   Nate Milkosky */
#include "generator.h"
#include "simulator.h"
#include <stdio.h>
#include <stdlib.h>
// Structure to keep track of the queue and the processes
typedef struct proc_block
{
	int timer; //keeps track of how long a process can run
	int pageStart; //where the pages are
	int pageEnd;
	struct proc_block *next; //next in queue
}proc_block;

//Function that creates a reference string that simulates a round robin scheduler. list is the ref
//string, runtime is the amount of time that will be spent processing,  timeQuant is the time quantum
//arrivaltimes is when the next process arrives, arrivallen is the length of that array, 
//procPositions is the bounds of each process, pages is the list of pages, pageLen is how many pages
void create_schedule(int *list, int runTime, int timeQuant, int *arrivalTimes, int arrivalLen, int *procPositions, int* pages, int pageLen)
{
	int clock = 0; //init clock
	proc_block *first = NULL; //init important queue items
	proc_block *last  = NULL;
	proc_block *cur   = NULL;	
	int nextArrival = 0; //keeps track of the process that will arrive next
	int queueLen = 0; //how many items in queue
	int listCounter = 0; //how many items in the ref string
	while( clock < runTime ) 
	{
		//check for arriving process	
		if(clock == arrivalTimes[nextArrival]) //if the clock is at the same time
		{ //as the designated next arrival, we can admit it to the queue.
			if(queueLen == 0) //if queue is empty, initialize it
			{
				first = malloc(sizeof(proc_block)); //init first item
				first->pageStart = procPositions[nextArrival];
				first->pageEnd = procPositions[nextArrival+1];
				first->timer = timeQuant;
				first->next = NULL;
				cur = first; //cur is first
				last = first; //since only one item, last is first
				queueLen++;
				nextArrival++;	//move to next arrival
			}
			else //otherwise just add to end
			{
				last->next = malloc(sizeof(proc_block)); //init next item
				last->next->pageStart = procPositions[nextArrival];
				last->next->pageEnd = procPositions[nextArrival+1];
				last->next->timer = timeQuant;
				last->next->next = NULL;
				last = last->next; //reassign last 
				queueLen++;
				nextArrival++;
			}
		}
		
		if(queueLen > 0) //if we have an item in the queue, start processing
		{
			first->timer--;	//decrement timer
			if(first->timer <= 0) //once it reaches 0, create ref string
			{
				int chooser = rand() % 4; //choose what type of locality we will use
				int procPages = first->pageEnd - first->pageStart; //how many pages the process has
				int bounds[] = {0, procPages}; //the bounds for the process 
				int *pagesNew = pages + first->pageStart; // move pointer to 
				// where the process has its first page
				switch(chooser)
				{
					case 0:
					 generate_spatial(list + listCounter, timeQuant, bounds, 2, pagesNew, procPages);
					 break;
					case 1:
					 generate_temporal(list + listCounter, timeQuant, bounds, 2, pagesNew, procPages);
					 break;
					case 2:
					 generate_combo(list + listCounter, timeQuant, bounds, 2, pagesNew, procPages);
					 break;
					case 3:
					 generate_pr(list + listCounter, timeQuant, pagesNew, procPages);
					 break;
					default:
					 generate_pr(list + listCounter, timeQuant, pagesNew, procPages);
					 break;
				}
				listCounter += timeQuant; //increment listcounter
				first->timer = timeQuant; //reset timer
				if(queueLen > 1) //if there is multiple items, move the finished
				{ //process to the end of the queue;
					last->next = first;
					first = first->next;
					last->next->next = NULL;
					last = last->next;
				}
			}		
		}	
		clock++;//move forward in time	
	} 	
}

int main(int argc, char *argv[])
{
	srand(time(0)); //set seed
	int refLen = 250; //how long our string will be
	int framesSize = 10; //how long our data structure for the simulators will be
	int rString[refLen]; //init reference string
	int pages[100]; //create pages, then set each value equal to its index
	int i;
	for(i = 0; i < 100; i++)
	{
		pages[i] = i;
	}
	int arriving[] = {0, 24, 32, 40, 130}; //when processes will arive
	int procBounds[] = {0, 10, 30, 45, 70, 100}; //the bounds for the processes
	create_schedule(rString, refLen, 10, arriving, 5, procBounds, pages, 100); //create the 
	//reference string from the schedule
	for(i = 0; i < refLen; i++) //print the reference string
		printf("%d ", rString[i]);
	printf("\n");
	
	// print the statistics
	int pageFaultsFifo = sim_fifo(rString, refLen, framesSize);
	int pageFaultstLRU = sim_tLRU(rString, refLen, framesSize);
	int pageFaultssecC = sim_secC(rString, refLen, framesSize);
	printf("FIFO: %d \n", pageFaultsFifo);
	printf("tLRU: %d \n", pageFaultstLRU);
	printf("secC: %d \n", pageFaultssecC);

	return 0;		
}
