#ifndef FITNESS_H
#define FITNESS_H

#include "chromosome.h"

#define ACCOMP_JAZZ 0
#define ACCOMP_CLASS 1
#define TOTAL_ACCOMP 2

using namespace std;

chromosome* accompaniments[TOTAL_ACCOMP];

bool accompanimentSet = false;

void createAccompaniment();

#endif
