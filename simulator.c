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
	int listCounter = 0; //keeps track of the length of the list so it doesn't go over
	ll_node *root = NULL;
	ll_node *tail = NULL;
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
			if( cur->value == list[counter] ) //it is in the stack
			{
				isInStack = 1;
				if(top->value != cur->value) //if it's not at the top, we have to move
				{
					top->next = cur; //put cur on top
					if(bottom->value != cur->value) //if it's not on the bottom,
					{ //there is a previous node in the stack
						cur->prev->next = cur->next; //link cur's prev to next
						cur->next->prev = cur->prev; // link cur's next to cur's prev
					}
					else //it is at the bottom, so we have to assign new bottom
					{
						bottom = cur->next; //bottom is what was on top of cur
					}
					cur->prev = top; //since cur has no prev, set it to top.
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

typedef struct sec_node
{
	int value;
	int referenced;
	struct sec_node *next;
}sec_node;

int sim_secC( int *list, int length, int frames )
{
	int counter = 0; //counter for iterating thru ref string
	int pageFaults = 0; // page fault counter
	int queueLen = 0; //keeps track of length of the queue
	sec_node *first = NULL;	//first in the queue (the oldest)
	sec_node *last = NULL; //last in queue, newest
	sec_node *cur = NULL; //for iterating through the queue
	int isInQueue = 0; // 0 is false, anything else means true
	while(counter < length) //iterate thru the list
	{
		while( isInQueue == 0 && cur != NULL )
		{
			if(cur->value == list[counter])
			{
				//if it is in the queue, set set isInQueue	
				isInQueue = 1;
				cur->referenced = 1; //set the reference bit
			}
			else
			{
				//if it isnt, go to the next node
				cur = cur->next;	
			}
		}
		if( queueLen == 0 )
		{
			pageFaults++;
			first = malloc(sizeof(sec_node));
			first->value = list[counter];
			first->next = NULL;
			first->referenced = 0;
			cur = first;
			last = first;
			queueLen++;		
		}
		else if( isInQueue == 0 )
		{
			if( queueLen >= frames ) //queue full
			{
				sec_node *temp = first; //used to iterate thru list
				sec_node *temp2 = NULL; //previous item in list
				while( temp != NULL && temp->referenced != 0 ) //iterate until we find
				{ // a page that isn't referenced
					temp->referenced = 0;
					if(temp->value == first->value) //when we encounter the first
					{//item and it's reference bit is set, move it to the end
						first = temp->next;
						last->next = temp;
						last = temp;					
					}
					else
					{ //if we are at another item and its reference bit is set
						temp2->next = temp->next; //update item before it
						last->next = temp; //move it to the end
						last = temp; //update pointer to last element
					}

				}
				temp = first->next; //now we can dequeue, so hold the next item
				free(first); //remove the data from first
				first = temp; // set first equal to the item after first
				temp = NULL; // set temp = null
				free(temp); //free it
				queueLen--; //decrement queue size
				temp2 = NULL; //remove temp2
				free(temp2);			
			}
			last->next = malloc(sizeof(sec_node));
			last->next->value = list[counter];
			last->next->next = NULL;
			last->next->referenced = 0;

			last = last->next;
			
			pageFaults++;
			queueLen++;
		}
		counter++;
		cur = first;
		isInQueue = 0;
	}
	return(pageFaults);
}	
