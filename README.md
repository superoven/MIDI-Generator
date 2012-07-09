# MIDI-Generator

A genetic algorithm to create arbitrary 12 bar blues solos.

## Implementation Details

Solutions are represented by a chromosome class defined in chromosome.cpp and chromosome.h. They are essentially a wrapper for a vector of bytes with various assignment and altering methods. The population of chromosomes that are put under the natural selection of this algorithm are represented as a vector of chromosome objects.

Every byte is a 16th note that can be tied to past notes to make tones of different length. All chromosomes will be (1 + (16 * # of bars)) bytes long.

### First byte in the chromosome:

bits 0-3: tempo

bits 4-6: type of instrument

bit    7: Melody always 1, Accompaniment always 0

### All following bytes:

bits 0-5: Note

bits 6-7: Articulation/Rest/Tie

## Algorithm Details

First, generate a population of random chromosomes.

Then enter the main loop by which we:

* Determine each one's fitness and declare a solution if it exists

* Randomly generate the number of members of the original population that will survive

* Pit two random members against one another and with a certain probability, the more fit of them will go on (eg. it is possible for the less fit one to win)

* Once the survivors have been determined (it is possible for a particular winner to have moved on more than once), stochastically have them mate and mutate.

* Fill up the rest of the population with random new members

## Files

`chromosome.cpp` is the chromosome class file. It contains all the definitions of the chromosome class methods.

`chromosome.h` is the chromosome header file.

`fitness.cpp` handles the fitness evaluation of an arbitrary chromosome for the algorithm.

`genetic.cpp` the genetic algorithm.

`genetic.h` is the header file for the genetic algorithm.

`MIDI-output.cpp` converts the solution into a MIDI file.

`testchrom.cpp` is a test file to check the chromosome class functions.

`testmidi.cpp` is a test file for the MIDI output functionality.

## Todo

* Implement the fitness function
