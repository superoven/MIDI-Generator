/*
  -------------------------
  Genetic Algorithm

  Attempts to imitate the process of evolution in a computer to solve problems. In this case, creating jazz.

  Implementation Details:
  Solutions are represented by a chromosome class defined in chromosome.cpp and chromosome.h. They are essentially
  a wrapper for a vector of bytes with various assignment and altering methods.
  The population of chromosomes that are put under the natural selection of this algorithm are represented as a
  vector of chromosome objects.

  Algorithm Details:
  First, generate a population of random chromosomes.
  Then enter the main loop by which we:
  --Determine each one's fitness and declare a solution if it exists
  --Randomly generate the number of members of the original population that will survive
  --Pit two random members against one another and with a certain probability, 
  the more fit of them will go on (eg. it is possible for the less fit one to win)
  --Once the survivors have been determined (it is possible for a particular winner to have moved on more than once),
  stochastically have them mate and mutate.
  --Fill up the rest of the population with random new members
  -------------------------
*/

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>
#include <algorithm>
#include "header/chromosome.h"
#include "header/genetic.h"
#include "header/MIDI-output.h"
using namespace std;

#ifdef _OPENMP //Import parallelism library safely
#include <omp.h>
#else
#define omp_get_num_threads() 0
#define omp_get_thread_num() 0
#endif

int mutationCount = 0; //Number of mutations that occurred (for debugging)
int crossoverCount = 0; //Number of crossovers that occurred (for debugging)
int iterations = 0; //Number of iterations to find a solution

bool compare_fitness(const chromosome & e1, const chromosome & e2) //dummy function for sorting
{
  if( e1.fitness > e2.fitness ) return true;
  return false;
}

double roulette(int lo, int hi) { //Generate a double precision floating point between hi and lo
  return (((double)(rand()/((double)RAND_MAX + (double)1)) * (hi - lo)) + lo);
}

void statusReport(int iter, int mut, int cross, bool found, chromosome& answer, vector<chromosome>& population) //Tell us what happened
{
  if (found)
    {
      cout << "Found a solution!\n";
      cout << "Number of iterations:\t" << setw(7) << right << iter << endl;
      cout << "Number of mutations:\t" << setw(7) << right << mut << endl;
      cout << "Number of crossovers:\t" << setw(7) << right << cross << endl;
      cout << "Solution found:\n";
      createMidi(&answer,1,"midi/output.mid");
      cout << "Solution saved to midi/output.mid.\nPlay it with \'bin/timidity midi/output.mid\'\n";
      cout << "Entire population saved to midi directory.\n";
    }
  else
    {
      cout << "No solution found. Maximum iterations exceeded.\n";
      cout << "Number of iterations:\t" << setw(7) << right << iter << endl;
      cout << "Number of mutations:\t" << setw(7) << right << mut << endl;
      cout << "Number of crossovers:\t" << setw(7) << right << cross << endl;
      cout << "Entire population saved to midi directory.\n";
    }    
}

int main()
{
  srand(time(NULL)); //Seed the random number generator
  
  omp_set_num_threads(omp_get_max_threads());
  vector<chromosome> population; //Create the initial population of random chromosomes
  vector<chromosome> nextgen; //Same with the next generation
  population.reserve(POP_SIZE); //Reserve the memory for the chromosome vector
  nextgen.reserve(POP_SIZE); //Same with the next generation
  for(int i = 0; i < POP_SIZE; i++) population.push_back(chromosome(CHROMO_LENGTH)); //fill vector

  bool found = false; //True if we found an answer
  int answerindex = -1; //The index in the population of the winner
  int save = -1;  
  while (!found) //Primary Loop
    {
      #pragma omp parallel for
      for(int i = 0; i < POP_SIZE; i++)
	{
	  population[i].fitnessEval(); //Determine the fitness of each chromosome
	}
      for(int i = 0; i < POP_SIZE; i++) //Check all the fitness ratings
	{
	  if (population[i].fitness > FITNESS_THRESHOLD) //Did we find an answer?
	    {
	      found = true;
	      answerindex = i; //Save the index of the winner
	    }
          else if (population[i].fitness > save+10)
            {
              save = population[i].fitness;
              cout << "Best Fitness Score: " << setw(4) << right << save << "/1000 (Threshold: " << FITNESS_THRESHOLD << ")\r";
	      cout.flush();
            }
	}
      if (found) break; //If we have an answer, stop looking for one
      unsigned int survivors = (rand() % (POP_SIZE - POP_REMAIN_LOWBOUND)) + POP_REMAIN_LOWBOUND; //how many children will there be from the previous gen
      //Number will be between the lowbound and population size, not including the population size. 
      //Therefore, there will always be at least one new random member.

      while (nextgen.size() < survivors) //Natural Selection
	{
	  int winner;
	  int loser;
	  int comp1 = rand() % POP_SIZE; //Competitor 1
	  int comp2 = rand() % POP_SIZE; //Competitor 2
	  if (population[comp1].getFitness() > population[comp2].getFitness()) {winner = comp1; loser = comp2;} //who is more fit?
	  else {winner = comp2; loser = comp1;}
	  if (roulette(0,1) < WINNER_RATE) //Determines if the stronger one survives
	    {
	      nextgen.push_back(population[winner]);
	    }
	  else
	    {
	      nextgen.push_back(population[loser]);
	    }
	}

      int numsex = rand() % survivors; //How many times will we attempt to crossover and mutate?
      for (int i = 0; i < numsex; i++)
	{
	  int part1 = rand() % survivors; //Partner 1
	  int part2 = rand() % survivors; //Partner 2
	  if (roulette(0,1) < CROSSOVER_RATE) //Attempt crossover
	    {
	      chromosome::crossover(nextgen[part1], nextgen[part2]);
	      crossoverCount++;
	    }
	  if (roulette(0,1) < MUTATION_RATE) //Attempt mutation
	    {
	      nextgen[part1].mutate();
	      mutationCount++;
	    }
	}
      
      while (nextgen.size() < POP_SIZE) nextgen.push_back(chromosome(CHROMO_LENGTH)); //fill up the population with random chromosomes as needed
      iterations++;
      population = nextgen; //reassign so the algorithm is consistent
      nextgen.clear(); //clear the memory that we don't need
      if (iterations >= MAX_ITERATIONS) break; //Leave if the maximum number of iterations is exceeded
    }
  statusReport(iterations, mutationCount, crossoverCount, found, population[answerindex], population); //Tell us what happened

  sort(population.begin(),population.end(),compare_fitness); //sort population by fitness in descending order
  
  for (int i = 1; i < POP_SIZE; i++) //output the entire population because why the hell not
    {
      ostringstream name;
      chromosome temp = population[i];
      name << "midi/population" << setw((int)log10(POP_SIZE - 1) + 1) << setfill('0') << i << ".mid";
      createMidi(&temp,1,name.str());
    }
  return 0;
}
