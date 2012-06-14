/*  When the chromosome is passed into the MIDI function it will be converted
	into the an array of notes. These notes are then split into their specific
	note-on and note-off events to make sorting easier. These events are then
	sorted, parsed, and outputed to the MIDI file

	Call createMidi to output the file, returns 0 if no errors and returns a
	non-zero value if an error occurred
*/

#include "chromosome.h"
#include "string.h"
#include "cstdio"
#include "cstdlib"

typedef struct
{
	int start,end,pitch;
} note;

typedef struct
{
	int time;
	int note;
	char state;
} event;

int chromosomeNumNotes(chromosome C)
{
	int numNotes = 0;

	/* test for notes .... numNotes++ */

	return numNotes;
}

note *parseChromosome(chromosome C, int numNotes)
{
	note notes[numNotes];

	note tmp;

	int pos = 0;
	int current = 0;

	while(pos<C.getLength())
	{
		/* determine note info
		tmp.start = 
		tmp.end = 
		tmp.pitch = 
		*/
		notes[current++] = tmp;
	}

	return notes;
}

event *parseNotes(note notes[], int numNotes)
{
	event events[2*numNotes];

	event tmp;

	for(int i=0;i<numNotes;i++)
	{
		tmp.time = start;
		tmp.state = 0;
		tmp.note = i;

		event[2*i] = tmp;

		tmp.time = end;
		tmp.state = 1;

		event[2*i+1] = tmp;
	}

	return events;
}

void sortEvents(event events[], int len)
{
	event tmp;
	int smallest;

	for(int i=0;i<len;i++)
	{
		smallest = i;
		for(int j=i+1;j<len;j++)
		{
			if(events[j].time<events[smallest].time)
				smallest = j;
		}
	tmp = events[i];
	events[i] = events[smallest];
	events[smallest] = tmp;
	}
}

int outputFile(event events[], string file)
{
	return 0;
}

int createMidi(chromosome C, string file)
{
	note* notes;
	event* events;

	int numNotes = chromosomeNumNotes(C);

	notes = parseChromosomes(C,numNotes);

	events = parseNotes(notes,numNotes);

	sortEvents(events,2*numNotes);

	return outputFile(events,file);
}
