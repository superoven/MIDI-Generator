/*
  -------------------------
  Genetic Algorithm
  -------------------------
*/

#include <iostream>
#include <cstdlib>
#include <ctime>
#include "chromosome.h"
#include "genetic.h"
using namespace std;

float totalFitness = 0.0f;
int mutationCount = 0;
int crossoverCount = 0;
int iterations = 0;

bool foundSolution(chromosome* const pop);
void populate(chromosome* pop);
chromosome& roulette(chromosome* const pop);

int main()
{
  srand(time(NULL));
}  

bool foundSolution(chromosome* const pop)
{
  bool found = false;
  for(int i = 0; i < POP_SIZE; i++) {
    float fitness = fitnessEval(pop[i]);
    if (fitness > FITNESS_THRESHOLD) found = true;
    pop[i].setFitness(fitness);
    totalFitness += pop[i].getFitness();
  }
  return found;
}
  
  
