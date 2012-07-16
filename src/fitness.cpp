/*
  -------------------------
  Fitness Function
  -------------------------
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

	// Fitness Evaluation Code
	// Evaluated on Scale 0 - 1000

        // Points awarded for key tones (Scale 0 - 500)
        // Key Points = 500 * [(note points)/(# of bars * 40)]^2
        // If the music isn't like 90% in key, I want it to be fucked.

        // Points awarded for chord tones 1 - 3 - 5 - 7 (Scale 0 - 300)
        // Chord Points  = [-40/(# of bars)]*|note points-[(# of bars) * 7.5]|+300
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
