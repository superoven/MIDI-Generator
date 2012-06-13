/*
  -------------------------
  Chromosome Header File
  -------------------------
*/

#ifndef CHROMOSOME_H
#define CHROMOSOME_H

#define CROSSOVER_RATE 0.7
#define MUTATION_RATE 0.001
#define POP_SIZE 100
#define CHROMO_LENGTH 300
#define GENE_LENGTH 8
#define GENE_POSSIBILITIES 256 		//must be = 2^GENE_LENGTH 
#define MAX_GENERATIONS 400
#define PRINT_BYTES_PER_LINE 16

class chromosome
{
private:
  char* bytes;
  long length;
  double fitness;
  static int numchromosomes;

public:
  chromosome(int len); 					//constructor
  ~chromosome(); 					//deconstructor
  void printChromosome(); 				//print method
  char getByte(int num);
  long getLength();
  double getFitness();
  void setFitness(double arg);
  void mutate();					//mutate this chromosome

  static int getNumChromosomes() { return numchromosomes; }
  static void combine(chromosome a, chromosome b); 	//combine chromosome a and b
};

#endif
