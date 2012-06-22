#ifndef MIDIOUTPUT_H
#define MIDIOUTPUT_H

#include "chromosome.h"
#include <string>


using namespace std;

#define TICKS_PER_QUARTER  120 // Number of delta time ticks per quarter note
#define MELODY_BASE_VELOCITY 100 //Base MIDI velocity for melody notes
#define ACCOMP_BASE_VELOCITY 60  //Base MIDI velocity for accompaniment notes
#define DELTA_HARD 20	// Change in velocity for a hard accented note
#define LOW_TEMPO 180
#define HIGH_TEMPO 210
#define DELTA_TEMPO ((HIGH_TEMPO-LOW_TEMPO)/15)

int BPM;

typedef struct
{
	int start,end;
	char pitch,velocity;
	bool melody;
} note;

typedef struct
{
	int time;
	int note;
	char state;
} event;

int chromosomeNumNotes(chromosome C);

note *parseChromosome(chromosome C, int numNotes);

event *parseNotes(note notes[], int numNotes);

void sortEvents(event events[], int len);

int outputFile(string file, note notes[], event events[], int numEvents);

int createMidi(chromosome C[], int numChromosomes, string file);

#endif
