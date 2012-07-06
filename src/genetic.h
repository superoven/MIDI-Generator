/*
  -------------------------
  Genetic Algorithm Header File
  -------------------------
*/

#ifndef GENETIC_H
#define GENETIC_H

#define CHROMO_LENGTH 300
#define CROSSOVER_RATE 0.7
#define WINNER_RATE 0.9
#define MUTATION_RATE 0.001
#define POP_SIZE 100
#define POP_REMAIN_LOWBOUND 90
#define MAX_ITERATIONS 1000
#define FITNESS_THRESHOLD 400.0f

float fitnessEval(chromosome& A);

#endif
