/*
  -------------------------
  Genetic Algorithm
  -------------------------
*/

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <memory>
#include <assert.h>
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
  srand(time(NULL)); //Seed the random number generator

  //Create the initial population of random chromosomes
  vector<chromosome> population;
  population.reserve(POP_SIZE); //Reserve the memory for the chromosome vector
  for(int i = 0; i < POP_SIZE; i++) population.push_back(chromosome(CHROMO_LENGTH)); //fill vector
  
  //sanity check
  population[29].printChromosome();
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
