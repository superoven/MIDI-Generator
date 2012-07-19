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
        // Key Points = 500 * [(note points)/(# of bars * 40)]^n
        // If the music isn't like 90% in key, I want it to be fucked.
        // n = 2; raising n may be used in the future to more stringently enforce key.

        // Points awarded for chord tones 1 - 3 - 5 - 7 (Scale 0 - 300)
        // Chord Points  = [-300/(# of bars * m)]*|note points-[(# of bars) * m]|+300
        // m = 7.5; This puts the max points at 75% chord tones, with points decreasing linearly
        // for higher or lower values. Changing m adjusts the optimal number of points without 
        // altering the solution note points = 0.
        
        // Points awarded for rhythmic not-shittiness (Scale 0 - 200)
	// Rhythm Points = 200 * [(note points)/(# of bars * 40)]^p
	// p = 1; pretty much the same deal as Key Points.

        // "Note points" for key and chords given by:
        //                      1       e       and     a
        //	----------------------------------------------
        // In Key/Chord         5       1       3       1       
        // Rest                 2       1       1       1
        // Deviating            0       0       0       0
        // This allows for an easy max of 10 points per beat.

	// "Note points" for rhythm given by:
	//			1	e	and	a
	//	----------------------------------------------
	// Hard attack		2	0	2	0
	// Normal attack	3	1	3	1
	// Tie			2	2	3	2
	// Rest			0	0	0	0
	// Also out of 10.
	// This is way simplistic, and at least a little arbitrary. I've completely avoided
	// larger structures and focused only on avoiding the worst.
	// Since our tempo is generally fast, avoiding articulation of the up-sixteenths 
	// seems like a good way to avoid nonsense. I've also decided to punish rests some more.
	// Keep in mind that this chart is "pre-swing," that is, articulations on 1 + and should
	// result in a pair of swung eighths.
	
	int inKey(int note, int key) // key is unused, it'll always be evaluated as C for now
	{

	}

	int inChord(int note, int key, int chord) // key - see above
	{

	}

	int note_score(int note, int articulation, int timing, int chord=0, int key=0)
	{
		// ignoring chord input evaluates note in key
		if (articulation==0) {
			if (timing==0)
				return 2;
			return 1;
		}
		int foo;
		if (chord==0)
			foo = inKey(note,key);
		else
			foo = inChord(note,key,chord);
		if (foo==0)
			return 0;
		if (timing%2==1)
			return 1;
		if (timing==2)
			return 3;
		return 5;
	}

	int rhythm_score(int articulation, int timing)
	{
		// This is just me hard-coding scores
		if (articulation==0)
			return 0;
		if (articulation==1) {
			if (timing==2)
				return 3;
			return 2;
		}
		if (timing%2==0) {
			if (articulation==2)
				return 3;
			return 2;
		}
		if (articulation==2)
			return 1;
		return 0;
	}
	
	fitness = 0;

	bytes[0] = (bytes[0] | 1); // make sure melody
}

void createAccompaniment()
{
	// create the accompaniment chromosomes
	printf("Creating accompaniment\n");
	//accompanimentSet = true;
}
