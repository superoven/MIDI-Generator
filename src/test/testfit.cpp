#include <iostream>
#include <cstdlib>
#include "../header/chromosome.h"
#include "../header/fitness.h"
using namespace std;

int main()
{
	srand(time(NULL));

	cout<<"Creating random chromosome\n";

	chromosome melody(12+1);

	cout<<"Printing out the chrosome\n";

	melody.printChromosome();

	cout<<"Finding chromosome fitness and make sure it's a melody chromosome\n";

	melody.fitnessEval();

	cout<<"Printing out chromosome now set to melody with it's calculated fitness\n";

	melody.printChromosome();

	return 0;
}
