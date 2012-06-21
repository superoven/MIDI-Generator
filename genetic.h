/*
  -------------------------
  Genetic Algorithm Header File
  -------------------------
*/

#ifndef GENETIC_H
#define GENETIC_H

#define CROSSOVER_RATE 0.7
#define MUTATION_RATE 0.001
#define POP_SIZE 100
#define MAX_GENERATIONS 400

float fitnessEval(chromosome& A);

#endif
