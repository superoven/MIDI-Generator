//This is where you come in Aaron.

#include "chromosome.h"

void chromosome::fitnessEval()
{
	fitness = 0;
	if(length<=0)
		return;

	// If the chromosome has any length to it, set it to melody and evaluate
	// it's fitness

	// fitness eval code

	fitness = 0;

	bytes[0] = (bytes[0] | 1); // make sure melody
}

void createAccompaniment()
{
	// create the accompaniment chromosomes
}
