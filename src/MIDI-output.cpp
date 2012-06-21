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

using namespace std;

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
	//static note notes[numNotes];
	static note * notes;

	notes = (note *) malloc(sizeof(note)*numNotes);
	if(notes == NULL)
		return NULL;

	note tmp;

	int pos = 0;
	int current = 0;

	while(pos<(int)(C.getLength()))
	{
		/* determine note info
		tmp.start = 
		tmp.end = 
		tmp.pitch = 35 + some num 0 <--> 63
		*/
		notes[current++] = tmp;
	}

	return notes;
}

event *parseNotes(note notes[], int numNotes)
{
	//static event events[2*numNotes];
	static event * events;

	events = (event *) malloc(sizeof(event)*2*numNotes);
	if(events == NULL)
		return NULL;

	event tmp;

	for(int i=0;i<numNotes;i++)
	{
		tmp.time = notes[i].start;
		tmp.state = 0;
		tmp.note = i;

		events[2*i] = tmp;

		tmp.time = notes[i].end;
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

int outputFile(string file, note notes[], event events[], int numEvents)
{
	FILE* out;

	out = fopen(file.c_str(), "wb");
	if(out == NULL)
		return -1;
	/* Header info */
	char chunkID[] = "MThd";	// Header name
	char chunkSize[4] = {0,0,0,6};	// size of head
	char formatType[2] = {0,0};		// Type of midi 0/1/2
	char numTracks[2] = {0,1};		
	uint16_t ticks_per_quarter = 120;	// Number of delta time ticks per
										// quarter note
	char timeDivision[2] = {(ticks_per_quarter>>8)&127, // make sure MSB is 0
							ticks_per_quarter&255};

	fwrite(chunkID, 4, 1, out);
	fwrite(chunkSize, 1, 4, out);
	fwrite(formatType, 1, 2, out);
	fwrite(numTracks, 1, 2, out);
	fwrite(timeDivision, 1, 2, out);

	/* Track info */
	uint32_t track_len = 0;
	char trackData[4294967296]; // tracklen is 4 bytes long, buffer of max len
	uint32_t endOfTrack = 0x00FF2F00;

	/* Conversion of notes into MIDI events */
	for(int i=0;i<numEvents;i++)
	{
		/* 	delta time code, variable length MSB is 0 if more data follows,
			1 if end of data
		*/

		if(i==0)
			trackData[track_len++] = 0x00;
		else
		{
			int dtime_len;
			uint32_t diff_time = events[i].time - events[i-1].time;
			char dtime[4] = {(diff_time>>21)&127,
							 (diff_time>>14)&127,
							 (diff_time>>7)&127,
							  diff_time&127};
			for(int i=3;i>=0;i--)
				if(dtime[i]==0)
					dtime_len = 4-i;
			for(int i=4-dtime_len;i<4;i++)
			{
				if(i<3)
					dtime[i]=dtime[i] | 1<<7; // MSB must be 1 if another byte follows
				trackData[track_len++] = dtime[i];
			}
		}

		if(events[i].state==0)
			trackData[track_len++] = 0x80;
		else
			trackData[track_len++] = 0x90;
		trackData[track_len++] = notes[events[i].note].pitch;
		trackData[track_len++] = 0x00;
	}

	char trackID[] = "MTrk";
	char trackSize[4] = {((track_len>>24)&255),
						 ((track_len>>16)&255),
						 ((track_len>>8)&255),
						 (track_len&255)};

	fwrite(trackID, 4, 1, out);
	fwrite(trackSize, 1, 4, out);

	fwrite(trackData, 1, track_len, out);
	fwrite(&endOfTrack, 4, 1, out);

	/* !!!!! FREEING NOTES AND EVENTS !!!!! */

	free(notes);
	free(events);

	return 0;
}

int createMidi(chromosome C, string file)
{
	note* notes;
	event* events;

	int numNotes = chromosomeNumNotes(C);

	notes = parseChromosome(C,numNotes);
	if(notes==NULL)
		return -1;

	events = parseNotes(notes,numNotes);
	if(events==NULL)
	{
		free(notes);
		return -1;
	}

	sortEvents(events,2*numNotes);

	/* !!!!! outputFile FREES NOTES AND EVENTS !!!!! */

	return outputFile(file,notes,events,2*numNotes);
}
