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

        // Points awarded for key tones (Scale 0 - 500)
        // Key Points = (50 * note points^2)/(# of bars * 4) 
        // If the music isn't like 90% in key, I want it to be fucked.

        // Points awarded for chord tones 1 - 3 - 5 - 7 (Scale 0 - 300)
        // Chord Points  = [-10/(# of bars)]*|note points-[(# of bars) x 30]|+300
        // This puts the max points at 75% chord tones, with points decreasing linearly
        // for higher or lower values.

        // These formulas are carefully derived but certainly inaccurate.

        // "Note points" given by:
        //                      1       e       and     a
        //      ----------------------------------------------
        // In Key/Chord         5       1       3       1       
        // Rest                 2       1       1       1
        // Deviating            0       0       0       0
        // This allows for an easy max of 10 points per beat.

        // I need at least the last 200 points for random shit. I'll cut into chord tones if necessary.
        // Basically I'm going to use this for punishing stuff that's static, bloopy, etc.

	fitness = 0;

	bytes[0] = (bytes[0] | 1); // make sure melody
}

void createAccompaniment()
{
	// create the accompaniment chromosomes
	printf("Creating accompaniment\n");
	//accompanimentSet = true;
}
