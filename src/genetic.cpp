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
#include "MIDI-output.h"
using namespace std;

int mutationCount = 0; //Number of mutations that occurred (for debugging)
int crossoverCount = 0; //Number of crossovers that occurred (for debugging)
int iterations = 0; //Number of iterations to find a solution

void populate(vector<chromosome> const *); //Make a new generation of chromosomes
chromosome& roulette(vector<chromosome> const *); //Stochastically determine which chromosome will move on

int main()
{
  srand(time(NULL)); //Seed the random number generator
  
  vector<chromosome> population; //Create the initial population of random chromosomes
  population.reserve(POP_SIZE); //Reserve the memory for the chromosome vector
  for(int i = 0; i < POP_SIZE; i++) population.push_back(chromosome(CHROMO_LENGTH)); //fill vector

  bool found = false;
  int answerindex = -1; //The index in the population of the winner
  double totalFitness = 0; //The total fitness of a given population
  
  while (!found) //Primary Loop
    {
      for(int i = 0; i < POP_SIZE; i++)
	{
	  population[i].fitnessEval(); //Determine the fitness of each chromosome
	  double checkval = population[i].getFitness(); //Save this variable so we don't have to dereference twice
      	  totalFitness += checkval; //Keep track of what the total fitness is
	  if (checkval > FITNESS_THRESHOLD) //Did we find an answer?
	    {
	      found = true;
	      answerindex = i; //Save the index of the winner
	    }
	}
      break; //For testing purposes only. This is obviously not complete
    }
  population[26].printChromosome(); //For sanity checking only
}
