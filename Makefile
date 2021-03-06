SOURCEFILES := genetic.cpp chromosome.cpp MIDI-output.cpp fitness.cpp
SOURCES := $(addprefix src/, $(SOURCEFILES))
HEADERS := $(addprefix src/header/, chromosome.h genetic.h MIDI-output.h fitness.h)
TESTMIDI := $(addprefix midi/, chromatic.mid notes.mid accents.mid tonguing.mid swung.mid mix.mid)
TESTEXE := testchrom testmidi testfit
PLAYBACK := ./bin/timidity		#Link to ubuntu midi playback binary, change this if you compiled it yourself

TCHROM := testchrom.cpp chromosome.cpp
TCHROMOBJ := $(addprefix obj/, $(TCHROM:.cpp=.o))

TMIDI := MIDI-output.cpp testmidi.cpp chromosome.cpp
TMIDIOBJ := $(addprefix obj/, $(TMIDI:.cpp=.o))

TFIT := testfit.cpp fitness.cpp chromosome.cpp MIDI-output.cpp
TFITOBJ := $(addprefix obj/, $(TFIT:.cpp=.o))

CC := g++
CFLAGS := -g -Wall -fopenmp
OBJECTS := $(addprefix obj/, $(SOURCEFILES:.cpp=.o))
EXECUTABLE := genetic

all: $(SOURCES) $(EXECUTABLE) $(HEADERS)

testchrom: $(TCHROMOBJ) $(HEADERS)
	$(CC) $(CFLAGS) $(TCHROMOBJ) -o $@
	./$@

testmidi: $(TMIDIOBJ) $(HEADERS)
	$(CC) $(CFLAGS) $(TMIDIOBJ) -o $@

testfit: $(TFITOBJ) $(HEADERS)
	$(CC) $(CFLAGS) $(TFITOBJ) -o $@
	./$@

$(EXECUTABLE): $(OBJECTS) $(HEADERS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@
	./$@
	timidity midi/output.mid -Ow -o midi/out.wav

obj/%.o: src/%.cpp src/header/%.h	#Force recompile if associated header changes
	$(CC) $(CFLAGS) -c $< -o $@

obj/%.o: src/%.cpp		#Overloaded object compiler for .cpp files without headers
	$(CC) $(CFLAGS) -c $< -o $@

obj/%.o: src/test/%.cpp		#Overloaded object compiler for test files
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TCHROMOBJ) $(TMIDIOBJ) $(TFITOBJ) $(EXECUTABLE) $(TESTMIDI) $(TESTEXE) src/*~ src/test/*~ src/header/*~ midi/population* midi/output.mid midi/out.wav
