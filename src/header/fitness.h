#ifndef FITNESS_H
#define FITNESS_H

#include "chromosome.h"

#define ACCOMP_JAZZ 0
#define ACCOMP_CLASS 1
#define TOTAL_ACCOMP 2

using namespace std;
int inKey(int note, int key);

int inChord(int note, int key, int chord);

int note_score(int note, int key, int articulation, int timing, int chord = 0);

int rhythm_score(int articulation, int timing);

void createAccompaniment();

#endif
