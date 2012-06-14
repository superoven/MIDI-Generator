/*  When the chromosome is passed into the MIDI function it will be converted
	into the an array of notes. These notes are then split into their specific
	note-on and note-off events to make sorting easier. These events are then
	sorted, parsed, and outputed to the MIDI file

	Call createMidi to output the file, returns 0 if no errors and returns a
	non-zero value if an error occurred
*/

#include "chromosome.h"
#include <string>
#include <cstdio>
#include <cstdlib>
#include <stdint.h>
#include <inttypes.h>

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

		events[2*i] = tmp;

		tmp.time = end;
		tmp.state = 1;

		events[2*i+1] = tmp;
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

int outputFile(string file, event events[], int numEvents)
{
	FILE* out;

	out = fopen(file.c_str(), "wb");
	if(out == NULL)
		return -1;
	/* Header info */
	char chunkID[4] = "MThd";
	char chunkSize[4] = {0,0,0,6};
	char formatType[2] = {0,0}
	char numTracks[2] = {0,1}
	char timeDivision[2] = {0x01,0xE0}

	fwrite(chunkID, 1, 4, out);
	fwrite(chunkSize, 4, 1, out);
	fwrite(formatType, 2, 1, out);
	fwrite(numTracks, 2, 1, out);
	fwrite(timeDivision, 2, 1, out);

	/* Track info */
	uint32_t track_len = 0;
	char trackData[4294967296];
	char endOfTrack = 0xFF;

	/* Conversion of notes into MIDI events */
	for(int i=0;i<numEvents;i++)
	{
		if(events[i].state==0)
			trackData[track_len++] = 0x80
		else
			trackData[track_len++] = 0x90
		trackData[track_len++] = events[i].note.pitch%128;
		trackData[track_len++] = 0x00;
	}

	chunkID = "MTrk";
	chunkSize = {((track_len>>24)&255),
				((track_len>>16)&255),
				((track_len>>8)&255),
				(track_len&255)};

	fwrite(trackData, tracklen, 1, out);
	fwrite(endOfTrack, 1, 1, out);

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

	return outputFile(file,events,2*numNotes);
}
