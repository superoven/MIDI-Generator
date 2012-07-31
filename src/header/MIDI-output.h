#ifndef MIDIOUTPUT_H
#define MIDIOUTPUT_H

#include "chromosome.h"
#include <string>

using namespace std;

#define TICKS_PER_QUARTER  120 // Number of delta time ticks per quarter note
#define MELODY_BASE_VELOCITY 100 //Base MIDI velocity for melody notes
#define ACCOMP_BASE_VELOCITY 60  //Base MIDI velocity for accompaniment notes
#define DELTA_HARD 20	// Change in velocity for a hard accented note
#define LOW_TEMPO 150
#define HIGH_TEMPO 180
#define DELTA_TEMPO ((HIGH_TEMPO-LOW_TEMPO)/15)

struct note
{
	int start,end;
	char pitch,velocity;
	bool melody;
};

typedef struct note note_t;

struct event
{
	int time;
	int note;
	char state;
};

typedef struct event event_t;

int chromosomeNumNotes(chromosome &C);

void parseChromosome(chromosome &C, note notes[], int numNotes);

void parseNotes(note notes[], event events[], int numNotes);

void sortEvents(event events[], int len);

int outputFile(string file, note* notes[], event* events[], int numEvents[], int numTracks);

int createMidi(chromosome C[], int numChromosomes, string file);

#endif
