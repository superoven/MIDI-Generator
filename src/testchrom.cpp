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
  becky.setFitness(2.56);
  assert(chromosome::getNumChromosomes() == 3);
  assert(becky.getLength() == 32);
  assert(becky.getFitness() == 2.56);
  
  cout << "Printing out \'Becky\':\n";
  becky.printChromosome();

  cout << "\nMutate \'Becky\':\n";
  becky.mutate();
  becky.printChromosome();

  cout << "\nAll sanity tests passed!\n";
  return 0;
}
