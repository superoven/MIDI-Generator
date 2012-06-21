/*
  -------------------------
  Chromosome Header File
  -------------------------
*/

#ifndef CHROMOSOME_H
#define CHROMOSOME_H

#define CHROMO_LENGTH 300
#define GENE_LENGTH 8
#define GENE_POSSIBILITIES 256 		//must be = 2^GENE_LENGTH 
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
  void setByte(int num, char A);
  void setFitness(double arg);
  void mutate();					//mutate this chromosome

  static int getNumChromosomes() { return numchromosomes; }
};

void crossover(chromosome& a, chromosome& b);

#endif
