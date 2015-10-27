/* CSC 345 Reference String Project
   Part 4, Scheduler Simulator
   Nate Milkosky */
#include "generator.h"
#include "simulator.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct proc_block
{
	int timer;
	int pageStart;
	int pageEnd;
	struct proc_block *next;
}proc_block;

void create_schedule(int *list, int runTime, int timeQuant, int *arrivalTimes, int arrivalLen, int *procPositions, int* pages, int pageLen)
{
	int clock = 0;
	proc_block *first = NULL;
	proc_block *last  = NULL;
	proc_block *cur   = NULL;	
	int nextArrival = 0;
	int queueLen = 0;
	int listCounter = 0;
	while( clock < runTime ) 
	{
		//check for arriving process	
		if(clock == arrivalTimes[nextArrival]) //if the clock is at the same time
		{ //as the designated next arrival, we can admit it to the queue.
			printf("  %d\n", nextArrival);
			if(queueLen == 0) //if queue is empty, initialize it
			{
				first = malloc(sizeof(proc_block));
				first->pageStart = procPositions[nextArrival];
				first->pageEnd = procPositions[nextArrival+1];
				first->timer = timeQuant;
				first->next = NULL;
				cur = first;
				last = first;
				queueLen++;
				nextArrival++;	
			}
			else
			{
				last->next = malloc(sizeof(proc_block));
				last->next->pageStart = procPositions[nextArrival];
				last->next->pageEnd = procPositions[nextArrival+1];
				last->next->timer = timeQuant;
				last->next->next = NULL;
				last = last->next;
				queueLen++;
				nextArrival++;
			}
		}
		
		if(queueLen > 0) //if we have an item in the queue, start processing
		{
			first->timer--;	
			if(first->timer <= 0)
			{
				int chooser = rand() % 4;
				int procPages = first->pageEnd - first->pageStart;
				int bounds[] = {0, procPages};
				int *pagesNew = pages + first->pageStart;
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
				listCounter += timeQuant;
				first->timer = timeQuant;
				if(queueLen > 1)
				{
					last->next = first;
					first = first->next;
					last->next->next = NULL;
					last = last->next;
				}
			}		
		}	
		clock++;		
	} 	
}

int main(int argc, char *argv[])
{
	int rString[250];
	int pages[100];
	int i;
	for(i = 0; i < 100; i++)
	{
		pages[i] = i;
	}
	int arriving[] = {0, 24, 32, 40, 130};
	int procBounds[] = {0, 10, 30, 45, 70, 100};
	create_schedule(rString, 250, 10, arriving, 5, procBounds, pages, 100); 
	for(i = 0; i < 250; i++)
		printf("%d ", rString[i]);
	printf("\n");
	return 0;		
}
