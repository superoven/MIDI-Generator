/*
  -------------------------
  Fitness Function
  -------------------------
*/

#include "header/chromosome.h"
#include "header/fitness.h"
#include <stdio.h>
#include <cmath>

void chromosome::fitnessEval()
{
	/*if(!accompanimentSet)
		createAccompaniment();*/
	fitness = 0;
	if(length<=0)
		return;

	if(length<17) // Less than 1 bar of music
		return;
	
	if((length%16)!=1)
	  printf("The last bar of music is not a full bar\n");
	
	int num_bars = (length-1)/16;
	
	if((num_bars%12)!=0)
	  printf("The number of bars is not a multiple of 12\n");
	
	int chords[12] = {1,1,1,1,4,4,1,1,5,4,1,1};

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
	
	const double m = 7.5;
	const double n = 2.0;
	const double p = 1.0;

	double key_note,chord_note,rhythm_note,key,chord,rhythm,penalty;

	key_note = chord_note = rhythm_note = penalty = 0;
	key = chord = rhythm = 0;

	//parse chromosome
	int pos = 1;
	int bar = 0;
	int timing = 0;

	int prev_note = -1;

	while(pos<length)
	{
		int note = (int)(bytes[pos]+128) >> 2;
		int articulation = bytes[pos] & 3;

		key_note += note_score(note, 1, articulation, timing%4);
		rhythm_note += rhythm_score(articulation, timing%4);
		chord_note += note_score(note, 1, articulation, timing%4, chords[bar]);

		if((prev_note>=0)&&(abs((double)(note-prev_note))>12))
			penalty += 10;

		pos++;
		timing++;
		if(timing==16)
		{
			bar++;
			timing=0;
		}
		if(bar==12)
			bar=0;
	}

	key = 500 * pow((key_note/(num_bars*40)),n);
	chord = (-300/(num_bars * m)) * abs(chord_note-(num_bars * m)) + 300;
	rhythm = 200 * pow((rhythm_note/(num_bars*40)),p);

	fitness = key+chord+rhythm-penalty;

	bytes[0] = (bytes[0] | 1); // make sure melody
}

int inKey(int note, int key)
{
	int bar = note%12;
	if ((bar==(1+key)%12)||(bar==(6+key)%12)||(bar==(8+key)%12))
		return 0;
	return 1;
}

int inChord(int note, int key, int chord)
{
	int foo;
	int bar = note%12;
	if (chord==1)
		foo = 0;
	else if (chord==4)
		foo = 5;
	else
		foo = 7;
	int tsum = key+foo;
	if ((bar==tsum%12)||(bar==(tsum+4)%12)||(bar==(tsum+7)%12)||(bar==(tsum+10)%12))
		return 1;
	return 0;
	
}

int note_score(int note, int key, int articulation, int timing, int chord)
{
	// ignoring chord input evaluates note in key
	// key can be set as any tone number of the root (the first C is 000001)
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

void createAccompaniment()
{
	// create the accompaniment chromosomes
	printf("Creating accompaniment\n");
	//accompanimentSet = true;
}
