/*  Call createMidi to output the file, returns 0 if no errors and returns a
	non-zero value if an error occurred

	createMidi is called with an array of chromosomes so that the melody and
	accompaniment can be outputed with 1 function call

	When the chromosome is parsed, it will be converted	into the an array of
	notes. These notes are then split into their specific note-on and note-off
	events to make sorting easier. These events are then sorted, parsed, and
	outputed to the MIDI file
*/

#include "MIDI-output.h"
#include "chromosome.h"
#include <string>
#include <cstdio>
#include <cstdlib>
#include <stdint.h>
#include <inttypes.h>

using namespace std;

int BPM;

int chromosomeNumNotes(chromosome &C)
{
	int numNotes = 0;
	char tmp;

	for(int i=1;i<C.getLength();i++)
	{
		tmp = C.getByte(i);
		if(tmp&(1<<1)) // Bit 7 being high means new articulation and new note
			numNotes++;
	}

	return numNotes;
}

void parseChromosome(chromosome &C, note notes[], int numNotes)
{
	bool melody = C.getByte(0)&1;
	BPM = LOW_TEMPO + DELTA_TEMPO*((unsigned char)(C.getByte(0)>>4));

	note tmp;
	unsigned char tmp_byte;

	int pos = 1;
	int current = 0;
	int len = C.getLength();

	bool triplet = false;
	uint32_t triplet_start = 0;

	while(pos<len)
	{
		if((pos&3)==0)
			triplet = false;
		/* determine note info */
		tmp_byte = C.getByte(pos);
		if(tmp_byte&(1<<1))
		{
			if(((pos&3)==1)&&(C.getByte(pos+2)&(1<<1))&&((C.getByte(pos+3)&3)==0))
			{
				triplet = true;
				triplet_start = pos;
			}
			
			tmp.start = (pos-1)*(TICKS_PER_QUARTER/4);

			pos++;
			while(pos<len && ((C.getByte(pos)&3)==1))
			{
				pos++;
			}

			if(triplet)
				tmp.end = (triplet_start-1)*(TICKS_PER_QUARTER/4) + (pos-triplet_start)*(TICKS_PER_QUARTER/3);
			else
				tmp.end = (pos-1)*(TICKS_PER_QUARTER/4);

			tmp.pitch = 35 + (tmp_byte>>2);

			tmp.melody = melody;

			if(melody)
				tmp.velocity = MELODY_BASE_VELOCITY;
			else
				tmp.velocity = ACCOMP_BASE_VELOCITY;
			if(tmp_byte&1)
				tmp.velocity += DELTA_HARD;

			notes[current++] = tmp;
		}
		else
		{
			pos++;
		}
	}
}

void parseNotes(note notes[], event events[], int numNotes)
{
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
	char timeDivision[2] = {(TICKS_PER_QUARTER>>8)&127, // make sure MSB is 0
							TICKS_PER_QUARTER&255};
	uint32_t microsecs_per_quarter = (60*1000*1000)/BPM;
	char tempo[3] = {(microsecs_per_quarter>>16)&255,
					 (microsecs_per_quarter>>8)&255,
					 (microsecs_per_quarter)&255};

	fwrite(chunkID, 4, 1, out);
	fwrite(chunkSize, 1, 4, out);
	fwrite(formatType, 1, 2, out);
	fwrite(numTracks, 1, 2, out);
	fwrite(timeDivision, 1, 2, out);
;
	/* Track info */
	uint32_t track_len = 0;
	char * trackData = new char[4294967296]; // tracklen is 4 bytes long, buffer of max len
	uint32_t endOfTrack = 0x00FF2F00;

	// set tempo
	trackData[track_len++] = 0x00;
	trackData[track_len++] = 0xFF;
	trackData[track_len++] = 0x51;
	trackData[track_len++] = 0x03;
	for(int i=0;i<3;i++)
		trackData[track_len++] = tempo[i];

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
			trackData[track_len++] = 0x90; // note on
		else
			trackData[track_len++] = 0x80; // note off
		trackData[track_len++] = notes[events[i].note].pitch;	 // note
		trackData[track_len++] = notes[events[i].note].velocity; // hardness
		if(events[i].state!=0)
			trackData[track_len-1] = 0x00;
	}
	track_len+=4; // count end of track length
	char trackID[] = "MTrk";
	char trackSize[4] = {((track_len>>24)&255),
						 ((track_len>>16)&255),
						 ((track_len>>8)&255),
						 (track_len&255)};

	fwrite(trackID, 4, 1, out);
	fwrite(trackSize, 1, 4, out);

	fwrite(trackData, 1, track_len - 3, out); //end of track isn't in trackdata
	fwrite(&endOfTrack, 4, 1, out);

	//fclose(out);//Hey Dono, so by commenting this out, it stopped segfaulting. I guess at some point later it calls these things or something? but in any case, it doesn't crash so thats nice. this really shouldn't be a final fix though as I'm fairly sure this creates a memory leak. -Taylor
	

	//delete trackData;
	//delete notes;
	//delete events;

	return 0;
}

int createMidi(chromosome C[], int numChromosomes, string file)
{
	note* notes;
	event* events;

	// Testing, only look at first note

	int numNotes = chromosomeNumNotes(C[0]);

	notes = new note[numNotes];

	if(notes==NULL)
	{
		printf("3a\n");
		return 1;
	}

	parseChromosome(C[0],notes,numNotes);

	events = new event[2*numNotes];

	if(events==NULL)
	{
		delete [] notes;
		return 1;
	}

	parseNotes(notes,events,numNotes);

	sortEvents(events,2*numNotes);

	return outputFile(file,notes,events,2*numNotes);
}
