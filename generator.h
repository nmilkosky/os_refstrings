#ifndef GENERATOR_H
#define GENERATOR_H

//Generates the spatial reference string
void generate_spatial(int *list, int length, int *setsPosition, int spLength, int *pages, int pLength);

//Generates a temporal reference string
void generate_temporal(int *list, int length, int *setsPosition, int spLength, int *pages, int pLength);

//Generates a combination of temporal and spatial reference strings
void generate_combo(int *list, int length, int *setsPosition, int spLength, int *pages, int pLength);

//generates a pseudorandom reference string
void generate_pr(int *list, int length, int *pages, int pLength);

#endif
