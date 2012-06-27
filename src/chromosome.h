/*
  -------------------------
  Chromosome Header File
  -------------------------
*/

#ifndef CHROMOSOME_H
#define CHROMOSOME_H

#define GENE_LENGTH 8
#define GENE_POSSIBILITIES 256 		//must be = 2^GENE_LENGTH 
#define PRINT_BYTES_PER_LINE 16

#include <vector>

class chromosome
{
private:
  std::vector<char>bytes;				//Note that I have made this a vector instead to bypass double-free headaches
  long length;
  double fitness;
  static int numchromosomes;
  //chromosome(const chromosome& fuck_you); 		//Make the copy constructor shut up and never work

public:
  chromosome(int len); 					//constructor
  ~chromosome(); 					//deconstructor
  void printChromosome(); 				//print method
  char getByte(int num);
  long getLength();
  double getFitness();
  void setByte(int num, char A);
  void setFitness(double arg);
  void fitnessEval();
  void mutate();					//mutate this chromosome

  static int getNumChromosomes() { return numchromosomes; }
  static void crossover(chromosome& a, chromosome& b);
};

#endif
