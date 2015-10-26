/* CSC 345 Reference String Project
   Part 3, Reference String Simulator
   Nate Milkosky */
#include <stdio.h>
#include <stdlib.h>
// linked list node structure
typedef struct ll_node 
{
	int value;
	struct ll_node *next;
} ll_node;

/* sim_fifo takes a reference string (and its length) as parameters, and returns the
number of page faults genereated by using the FIFO page replacement strategy */
int sim_fifo(int *list, int length, int listLength)
{
	int counter = 0; // counter when iterating through the list
	int pageFaults = 0; // page fault counter
	int listLen = 0; //keeps track of the length of the list so it doesn't go over
	ll_node *root = NULL;
	ll_node *tail = NULL;
	int listCounter = 0;
	int isInList = 0; //0 is false, otherwise true	
	ll_node *cur = NULL;
	//iterate through list
	while (counter < length)
	{ //then iterate to check if it's in the list
		while(isInList == 0 && cur != NULL)
		{
			if(cur->value == list[counter])
			{
				// if it is, set isInList
				isInList = 1;
			}
			else
			{
				// if it's not, iterate to next node and check
				cur = cur->next;
			}

		}
		
		if (listCounter == 0) //empty list, so make the list
		{
			pageFaults++; //note a page fault
			root = malloc(sizeof(ll_node)); // allocate root
			root->value = list[counter]; // set value
			root->next = NULL; // first item has no next 
			cur = root; // set the current item to root
			tail = root; // set tail to root
			listCounter++; // increment number of items in list		
		}
		else if (isInList == 0) //not in list
		{
			if(listCounter >= listLength) //list is full
			{
				ll_node *temp = root->next; // if so, copy the next into temp
				free(root); //free the old root
				root = temp; // set root = to temp
				temp = NULL; // remove the pointer to root so we can free safely.
				free(temp); // free
				listCounter--; //decrement listCounter since we removed the root
			}  
			//since there is space, we can add item to the end
			tail->next = malloc(sizeof(ll_node)); 
			tail->next->value = list[counter];
			tail->next->next = NULL;
						
			//now update tail
			tail = tail->next;
			//update counters
			pageFaults++;
			listCounter++;
		}
		counter++; //move on to next
		cur = root; // reset current node to root for check loop
		isInList = 0; // reset isInList for check loop
	}
	return(pageFaults);	
}
// structure for stack nodes, has to keep track of previous and next (for removing from bottom)
typedef struct stack_node
{
	int value;
	struct stack_node *prev;
	struct stack_node *next;
} stack_node;

int sim_tLRU( int *list, int length, int frames)
{
	int counter = 0; //set up variables that we will need
	int pageFaults = 0;
	stack_node *top = NULL;
	stack_node *bottom = NULL;
	stack_node *cur = NULL;
	int isInStack = 0; // 0 if it's not in the list
	int stackSize = 0;
	while( counter < length ) //iterate thru list
	{
		while( isInStack == 0 && cur != NULL ) //iterate thru stack, check if page is in stack
		{ //iterates from bottom up
			if( cur->value == list[counter]) //it is in the stack
			{
				isInStack = 1;
				if(top->value != cur->value) //if it's not at the top, we have to move
				{
					top->next = cur; //put cur on top
					if(bottom->value != cur->value) //if it's not on the bottom,
					{ //there is a previous node in the stack
						cur->prev->next = cur->next; //link cur's prev to next
					}
					else //it is at the bottom, so we have to assign new bottom
					{
						bottom = cur->next; //bottom is what was on top of cur
					}
					cur->next->prev = cur->prev; // link cur's next to cur's prev
					top = cur; //reassign top
					cur->next = NULL; //top doesn't have a next
				}
			}
			else //not sure yet if it is in the stack
			{
				cur = cur->next; //move to next element
			}
		}

		if (stackSize == 0) //stack empty, so make initial stack
		{
			pageFaults++; //pagefault occurs
			top = malloc(sizeof(stack_node)); // allocate top
			top->value = list[counter]; // set value
			top->next = NULL; //first item has no prev or next
			top->prev = NULL;
			bottom = top; // set bottom of stack to the top
			cur = top; // current item is top
			stackSize++; //increment the counter for the number of items on stack
		}

		else if( isInStack == 0 ) //not in the stack
		{
			if( stackSize >= frames ) //stack is full
			{
				stack_node *temp = bottom->next; // copy the item one from the bottom
				free(bottom); //free the old bottom
				bottom = temp; // set the bottom to be the new bottom
				bottom->prev = NULL; // remove pointer to old bottom
				temp = NULL; //clean up so we can free
				free(temp); // free temp
				stackSize--; //decrement stacksize since we removed an item
			}
			//set up new top
			top->next = malloc(sizeof(stack_node));
			top->next->value = list[counter];
			top->next->next = NULL;
			top->next->prev = top;
			//reassign top
			top = top->next;
			//since we have to add, pagefault and increase size
			pageFaults++;
			stackSize++;  
		}
		counter++;
		cur = bottom;
		isInStack = 0;
	}
	return(pageFaults);	
}
int main ( int argc, char *argv[])
{
	int refString[] = { 1, 2, 1, 3, 4, 1 };
	int pageFaultsFifo = sim_fifo(refString, 6, 3);
	int pageFaultstLRU = sim_tLRU(refString, 6, 3);
	printf("FIFO: %d \n", pageFaultsFifo);
	printf("tLRU: %d \n", pageFaultstLRU);

	return 0;
}
