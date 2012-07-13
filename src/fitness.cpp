/*
  -------------------------
  Fitness Function
  -------------------------

  Ideas:
  So, I figured I would throw out some ideas I had for how this might work. Feel free to ignore them,
  it's just what I thought might help.

  Remember, in many AI applications, over evaluating can really constrict your results, and since our
  goal is to be able to make interesting melodies that aren't all the same, try to judge them as
  generally as possible. Try to just make sure it doesn't do anything completely dumb and let the
  computer be creative with it.

  Also, remember this needs to be deterministic. For a given chromosome, it must always give the same
  score; the stochastic nature of this project must be entirely contained in the genetic algorithm.

  We ought to pick a key/chord structure and stick with it, make it work with that, then worry about
  trying to make it generalizable to any key/chord structure.

  I think it may be a good idea to keep along an array with as many spaces as there are possible notes, 
  run through and count the number of times that each note appears, giving those with larger amounts of
  notes explicitly part of that key higher scores.

  Then, do this in smaller portions for bars, counting the notes and scoring it more highly if notes in
  that particular chord change are the most common ones.

  I'll post more ideas as I come up with them. Hope this helps you guys get started.
  -Taylor
*/

#include "header/chromosome.h"
#include "header/fitness.h"
#include <stdio.h>

void chromosome::fitnessEval()
{
	/*if(!accompanimentSet)
		createAccompaniment();*/
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
	printf("Creating accompaniment\n");
	//accompanimentSet = true;
}
