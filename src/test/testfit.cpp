#include <iostream>
#include <cstdlib>
#include "../header/chromosome.h"
#include "../header/fitness.h"
#include "../header/MIDI-output.h"
using namespace std;

int main()
{
	srand(time(NULL));

	cout<<"Creating random chromosome\n";

	chromosome melody(120+1);

	cout<<"Printing out the chromosome\n";

	melody.printChromosome();

	cout<<"Finding chromosome fitness and make sure it's a melody chromosome\n";

	melody.fitnessEval();

	cout<<"Printing out chromosome now set to melody with it's calculated fitness\n";

	melody.printChromosome();

	if (createMidi(&melody,1,"midi/test.mid")==0)
		cout<<"Music saved to /midi/test.mid\n";
	else
		cout<<"Error: Unable to save music";
	
	return 0;
}
