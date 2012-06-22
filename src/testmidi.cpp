#include <iostream>
#include <cstdlib>
#include "MIDI-output.h"
#include "chromosome.h"
using namespace std;

int main()
{
	char note;
	int pos;
	int note_len = 1;

	cout<<"Creating 16th note chromatic chromosome\n";

	chromosome chromatic(64+1);
	chromatic.setByte(0,1);
	for(note=0;note<64;note++)
		chromatic.setByte(note+1,(note<<2)+2);

	if(createMidi(&chromatic,1,"chromatic.mid")==0)
		cout<<"Saved to chromatic.midi\n";
	else
		cout<<"An error occurred with chromatic\n";

	cout<<"Creating one of every type of note\n";

	chromosome notes(5*16 + 1);
	notes.setByte(0,1);
	for(int i=0;i<5;i++)
	{
		pos = 0;
		notes.setByte(5*i + pos + 1, 2);
		for(pos=1;pos<note_len;pos++)
			notes.setByte(5*i + pos + 1, 1);
		for(;pos<16;pos++)
			notes.setByte(5*i + pos + 1, 0);
		note_len++;
	}

	if(createMidi(&notes,1,"notes.mid")==0)
		cout<<"Saved to notes.midi\n";
	else
		cout<<"An error occurred with notes\n";

	cout<<"Quarter notes, fastest tempo, alternating accents\n";

	chromosome accents(64+1);
	accents.setByte(0,(15<<4)+1);
	for(int i=0;i<(64/4);i++)
	{
		accents.setByte(i*4 + 1,2 + (i%2));
		accents.setByte(i*4 + 2,1);
		accents.setByte(i*4 + 3,1);
		accents.setByte(i*4 + 4,1);
	}

	if(createMidi(&accents,1,"accents.mid")==0)
		cout<<"Saved to accents.midi\n";
	else
		cout<<"An error occurred with accents\n";

	cout<<"Testing complete, check midi files to confirm accuracy\n";

	return 0;
}
