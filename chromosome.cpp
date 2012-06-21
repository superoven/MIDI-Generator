/*
  -------------------------
  Chromosome Class

  This initiallizes a random bit string of a defined length, and links various
  attributes to it, to make coding with the object easier.
  -------------------------
*/

#include <iostream>
#include <cstdlib>
#include <ctime>
#include "chromosome.h"
using namespace std;

int chromosome::numchromosomes = 0;

chromosome::chromosome(int len)
{
  //Constructor
  //If fitness is -1, it hasn't been set by the algorithm yet.

  bytes = new char[len];
  length = len;
  fitness = -1;
  numchromosomes++;
  for(int i = 0; i < len; i++) bytes[i] = (char)(rand() % GENE_POSSIBILITIES);
}

chromosome::~chromosome()
{
  //Deconstructor

  delete bytes;
  numchromosomes--;
}

void chromosome::printChromosome()
{
  //Print out everything about the chromosome

  cout << "Data:\n";
  int i;
  for(i = 0; i < length; i++)
    {
      int value1 = bytes[i] & 0xF;
      int value2 = (bytes[i] & 0xF0) >> 4;
      cout << hex << uppercase << value2 << value1 << (((i % PRINT_BYTES_PER_LINE) != PRINT_BYTES_PER_LINE - 1) ? " " : "\n");
    }
  if (i % PRINT_BYTES_PER_LINE) cout << endl;
  cout << "Length: " << dec << length << endl; 
  cout << "Fitness: " << dec << fitness << endl;
}

char chromosome::getByte(int num) { return bytes[num]; }

long chromosome::getLength() { return length; }

double chromosome::getFitness() { return fitness; }

void chromosome::setByte(int num, char A) { bytes[num] = A; }
  
void chromosome::setFitness(double arg) { fitness = ((arg > 0) ? arg : 0); }

void chromosome::mutate()
{
  //Flips a random bit in the chromosome
  //Also sets the fitness to -1
  
  int randbyte = rand() % length;
  int randbit = rand() % GENE_LENGTH;
  bytes[randbyte] ^= 1 << randbit;
  fitness = -1;
}

void crossover(chromosome& a, chromosome& b) 	//combine chromosome a and b
{
  int point = (int) rand() % a.getLength();
  int bufferlen = a.getLength() - point;
  char* buffer1 = new char [bufferlen];
  char* buffer2 = new char [bufferlen];

  for (int i = 0; i < bufferlen; i++) {
    buffer1[i] = a.getByte(point + i);
    buffer2[i] = b.getByte(point + i);
  }
  for (int i = 0; i < bufferlen; i++) {
    a.setByte(point + i, buffer2[i]);
    b.setByte(point + i, buffer1[i]);
  }
    
  delete buffer1;
  delete buffer2;
}
