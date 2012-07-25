/*
  -------------------------
  Genetic Algorithm Header File
  -------------------------
*/

#ifndef GENETIC_H
#define GENETIC_H

#define CHROMO_LENGTH 385 //two sets of 12 bar blues
#define CROSSOVER_RATE 0.9
#define WINNER_RATE 0.95
#define MUTATION_RATE 0.9 //I know that's high as balls, but man, it's working. Hilarious how I originally had it at 0.001
#define POP_SIZE 100
#define POP_REMAIN_LOWBOUND 98
#define MAX_ITERATIONS 1000000
#define FITNESS_THRESHOLD 950.0f

#endif
