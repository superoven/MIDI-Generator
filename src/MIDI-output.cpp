/*  Call createMidi to output the file, returns 0 if no errors and returns a
	non-zero value if an error occurred

	createMidi is called with an array of chromosomes so that the melody and
	accompaniment can be outputed with 1 function call

	When the chromosome is parsed, it will be converted	into the an array of
	notes. These notes are then split into their specific note-on and note-off
	events to make sorting easier. These events are then sorted, parsed, and
	outputed to the MIDI file
*/

/*! 
 * \file MIDI-output.cpp
 * \brief Create MIDI files out of chromosomes
 * \author Donovan McKelvey 
 * \date July 31, 2012
 */

#include "header/MIDI-output.h"
#include "header/chromosome.h"
#include <string>
#include <cstdio>
#include <cstdlib>
#include <stdint.h>
#include <inttypes.h>

using namespace std;

int BPM = LOW_TEMPO;

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

void parseChromosome(chromosome &C, note_t notes[], int numNotes)
{
  bool melody = C.getByte(0)&1;
  if(melody)
  	BPM = LOW_TEMPO + (((unsigned char)(C.getByte(0))>>4)*DELTA_TEMPO);

  note tmp;
  unsigned char tmp_bytes[4];

  int pos = 1;
  int current = 0;
  int len = C.getLength();
  int small_pos,stoe=0;

  bool triplet = false;
  bool tied = false;
  uint32_t beat_start = 0;

  while(pos<len)
    {
		if(current>=numNotes)
		{
			printf("you dun really goofed\t%d\n",current-numNotes);
		}
		for(int i=0;i<4;i++)
			tmp_bytes[i] = C.getByte(pos+i);

		small_pos = 0;
		triplet = false;
		beat_start = (pos-1)*(TICKS_PER_QUARTER/4);

		pos+=4;

		if((tmp_bytes[2]&(1<<1))&&((tmp_bytes[3]&3)<2)) // 3rd articulated, 4th not
			triplet = true;

		if(triplet)
		{
			if(tied)
			{
				while((tmp_bytes[small_pos]&3)==1) //articulation guarenteed, don't have to check bounds of array
					small_pos++;

				tmp.end = beat_start + small_pos*(TICKS_PER_QUARTER/3);

				stoe = tmp.end;

				notes[current++] = tmp;
				tied = false;
			}
			if(small_pos==0)
			{
				if(tmp_bytes[0]&(1<<1))
				{
					tmp.pitch = 35 + (tmp_bytes[0]>>2);

					tmp.melody = melody;

					if(melody)
						tmp.velocity = MELODY_BASE_VELOCITY;
					else
						tmp.velocity = ACCOMP_BASE_VELOCITY;
					if(tmp_bytes[0]&1)
						tmp.velocity += DELTA_HARD;

					tmp.start = beat_start;

					if(tmp.start<stoe)
						printf("you dun goofed\n");
				

					tmp.end = beat_start + (TICKS_PER_QUARTER/3);

					if((tmp_bytes[1]&3) == 1)
					{
						tmp.end += (TICKS_PER_QUARTER/3);
						small_pos++;
					}

					stoe = tmp.end;

					notes[current++] = tmp;
					tied = false;
				}
				small_pos++;
			}
			if(small_pos==1)
			{
				if(tmp_bytes[1]&(1<<1))
				{
					tmp.pitch = 35 + (tmp_bytes[1]>>2);

					tmp.melody = melody;

					if(melody)
						tmp.velocity = MELODY_BASE_VELOCITY;
					else
						tmp.velocity = ACCOMP_BASE_VELOCITY;
					if(tmp_bytes[1]&1)
						tmp.velocity += DELTA_HARD;

					tmp.start = beat_start + (TICKS_PER_QUARTER/3);

					if(tmp.start<stoe)
						printf("you dun goofed\n");

					tmp.end = beat_start + 2*(TICKS_PER_QUARTER/3);
					stoe = tmp.end;
					notes[current++] = tmp;
				}
				small_pos++;
			}
			if(small_pos==2)
			{
				tmp.pitch = 35 + (tmp_bytes[2]>>2);

				tmp.melody = melody;

				if(melody)
					tmp.velocity = MELODY_BASE_VELOCITY;
				else
					tmp.velocity = ACCOMP_BASE_VELOCITY;
				if(tmp_bytes[2]&1)
					tmp.velocity += DELTA_HARD;

				tmp.start = beat_start + 2*(TICKS_PER_QUARTER/3);

				if(tmp.start<stoe)
					printf("you dun goofed\n");

				if((pos<len)&&((C.getByte(pos)&3)==1))
					tied = true;
				else
				{
					tmp.end = beat_start + TICKS_PER_QUARTER;
					stoe = tmp.end;

					notes[current++] = tmp;
				}
			}
		}
		else
		{
			if(tied)
			{
				while((small_pos<4)&&((tmp_bytes[small_pos]&3)==1))
					small_pos++;
				if((small_pos==4)&&(pos<len)&&((C.getByte(pos)&3)==1))
					tied = true;
				else
				{
					tmp.end = beat_start + small_pos*(TICKS_PER_QUARTER/4);

					stoe = tmp.end;

					notes[current++] = tmp;
					tied = false;
				}
			}
			while(small_pos<4)
			{
				while((small_pos<4)&&((tmp_bytes[small_pos]&3)<2))
					small_pos++;
				if(small_pos<4)
				{
					tmp.pitch = 35 + (tmp_bytes[small_pos]>>2);

					tmp.melody = melody;

					if(melody)
						tmp.velocity = MELODY_BASE_VELOCITY;
					else
						tmp.velocity = ACCOMP_BASE_VELOCITY;
					if(tmp_bytes[small_pos]&1)
						tmp.velocity += DELTA_HARD;

					tmp.start = beat_start + small_pos*(TICKS_PER_QUARTER/4);

					if(tmp.start<stoe)
						printf("you dun goofed\n");

					small_pos++;
					while((small_pos<4)&&((tmp_bytes[small_pos]&3)==1))
						small_pos++;
					if((small_pos==4)&&(pos<len)&&((C.getByte(pos)&3)==1))
						tied = true;
					else
					{
						tmp.end = beat_start + small_pos*(TICKS_PER_QUARTER/4);

						stoe = tmp.end;

						notes[current++] = tmp;
						tied = false;
					}
				}
			}
		}
    }
	if((pos==len)&&(current<numNotes))
	{
		tmp.end = (pos-1)*(TICKS_PER_QUARTER/4);
		notes[current++] = tmp;
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

int outputFile(string file, note* notes[], event* events[], int numEvents[], int numTracks)
{
  FILE* out;

  out = fopen(file.c_str(), "wb");
  if(out == NULL)
    return -1;
  /* Header info */
  char chunkID[] = "MThd";	// Header name
  char chunkSize[4] = {0,0,0,6};	// size of head
  char formatType[2] = {0,1};		// Type of midi 0/1/2
  char num_tracks[2] = {((numTracks>>8)&255),(numTracks&255)};		
  char timeDivision[2] = {(TICKS_PER_QUARTER>>8)&127, // make sure MSB is 0
			  TICKS_PER_QUARTER&255};
  uint32_t microsecs_per_quarter = (60*1000*1000)/BPM;
  char tempo[3] = {(microsecs_per_quarter>>16)&255,
		   (microsecs_per_quarter>>8)&255,
		   (microsecs_per_quarter)&255};

  fwrite(chunkID, 4, 1, out);
  fwrite(chunkSize, 1, 4, out);
  fwrite(formatType, 1, 2, out);
  fwrite(num_tracks, 1, 2, out);
  fwrite(timeDivision, 1, 2, out);

  /* Track info */
  char * trackData = (char *) malloc(1073741824); // tracklen is 4 bytes long, buffer of max len
  char endOfTrack[4] = {0x00,0xFF,0x2F,0x00};

  for(int j=0;j<numTracks;j++)
  {
	  uint32_t track_len = 0;
	  if(j==0)
	  {
		  // set tempo
		  trackData[track_len++] = 0x00;
		  trackData[track_len++] = 0xFF;
		  trackData[track_len++] = 0x51;
		  trackData[track_len++] = 0x03;
		  for(int i=0;i<3;i++)
			trackData[track_len++] = tempo[i];
	  }

	  /* Conversion of notes into MIDI events */
	  for(int i=0;i<numEvents[j];i++)
		{
		  /* 	delta time code, variable length MSB is 1 if more data follows,
			0 if end of data
		  */

		  if(i==0)
			trackData[track_len++] = 0x00;
		  else
		{
		  int dtime_len=4;
		  uint32_t diff_time = events[j][i].time - events[j][i-1].time;
		  unsigned char dtime[4] = {(diff_time>>21)&127,
						(diff_time>>14)&127,
						(diff_time>>7)&127,
						diff_time&127};
		  for(int i=3;i>=0;i--)
			{
			  if(dtime[i]==0)
			{
			  dtime_len = max(1,3-i);
			  i=-1;
			}
			}
		  for(int i=4-dtime_len;i<4;i++)
			{
			  if(i<3)
			dtime[i]=dtime[i] | 1<<7; // MSB must be 1 if another byte follows
			  trackData[track_len++] = dtime[i];
			}
		}

		  if(events[j][i].state==0)
			trackData[track_len++] = 0x90; // note on
		  else
			trackData[track_len++] = 0x80; // note off
		  trackData[track_len++] = notes[j][events[j][i].note].pitch;	 // note
		  trackData[track_len++] = notes[j][events[j][i].note].velocity; // hardness
		  if(events[j][i].state!=0)
			trackData[track_len-1] = 0x00;
		}

	  char trackID[] = "MTrk";
	  char trackSize[4] = {((track_len>>24)&255),
				   ((track_len>>16)&255),
				   ((track_len>>8)&255),
				   (track_len&255)};

	  fwrite(trackID, 4, 1, out);
	  fwrite(trackSize, 1, 4, out);

	  fwrite(trackData, 1, track_len, out);
	  fwrite(endOfTrack, 1, 4, out);
  }

  fclose(out);	

  free(trackData);
  free(numEvents);
  for(int i=0;i<numTracks;i++)
  {
	free(notes[i]);
	free(events[i]);
  }
  free(notes);
  free(events);
  return 0;
}

int createMidi(chromosome C[], int numChromosomes, string file)
{

	note_t** track_notes;
	event_t** track_events;

	int* numEvents;

	track_notes = (note_t**) malloc(sizeof(note_t*) * numChromosomes);
	track_events = (event_t**) malloc(sizeof(event_t*) * numChromosomes);

	if((track_notes==NULL)||(track_events==NULL))
		return 1;

	numEvents = (int*) malloc(sizeof(int) * numChromosomes);

/*  note_t* notes;
  event_t* events;
*/
  // Testing, only look at first note

	for(int i=0;i<numChromosomes;i++)
	{
		int numNotes = chromosomeNumNotes(C[i]);
		numEvents[i] = 2*numNotes;

		track_notes[i] = (note_t*) malloc(sizeof(note_t) * numNotes);

		if(track_notes[i]==NULL)
			return 1;

		parseChromosome(C[i],track_notes[i],numNotes);

		track_events[i] = (event_t*) malloc(sizeof(event_t) * 2 * numEvents[i]);

		if(track_events[i]==NULL)
		{
			//free(notes);
			return 1;
		}

		parseNotes(track_notes[i],track_events[i],numNotes);

		sortEvents(track_events[i],numEvents[i]);
	}

	return outputFile(file,track_notes,track_events,numEvents,numChromosomes);
}
