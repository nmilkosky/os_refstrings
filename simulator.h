#ifndef SIMULATOR_H
#define SIMULATOR_H
//simulate fifo on a reference string, returns #pagefaults
int sim_fifo(int *list, int length, int listLength);

//simulate true LRU on a ref string, returns pagefaults
int sim_tLRU(int *list, int length, int frames);

//simulate second chance on a ref string, returns pagefaults
int sim_secC(int *list, int length, int frames);

#endif
