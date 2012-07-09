#include <iostream>
#include <ctime>
#include <cstdlib>
#include <assert.h>
#include "chromosome.h"
using namespace std;

int main()
{
  srand(time(NULL));

  chromosome jim(16);
  assert(chromosome::getNumChromosomes() == 1);
  assert(jim.getLength() == 16);
  assert(jim.getFitness() == -1);

  chromosome bob(16);
  cout << "Printing out \'jim\':\n";
  jim.printChromosome();
  cout << "Printing out \'bob\':\n";
  bob.printChromosome();
  cout << "Performing crossover on \'jim\' and \'bob\':\n";
  chromosome::crossover(jim,bob);
  cout << "Printing out \'jim\':\n";
  jim.printChromosome();
  cout << "Printing out \'bob\':\n";
  bob.printChromosome();
  assert(jim.getFitness() == -1);

  chromosome becky(32);
  assert(chromosome::getNumChromosomes() == 3);
  assert(becky.getLength() == 32);
  
  cout << "Printing out \'Becky\':\n";
  becky.printChromosome();

  cout << "Mutate \'Becky\':\n";
  becky.mutate();
  becky.printChromosome();

  cout << "\nAll sanity tests passed!\n\n";
  return 0;
}
