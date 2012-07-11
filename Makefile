SOURCEFILES := genetic.cpp chromosome.cpp MIDI-output.cpp fitness.cpp
SOURCES := $(addprefix src/, $(SOURCEFILES))
HEADERS := $(addprefix src/, chromosome.h genetic.h MIDI-output.h fitness.h)
TESTMIDI := $(addprefix midi/, chromatic.mid notes.mid accents.mid)
TESTEXE := testchrom testmidi testfit
PLAYBACK := ./bin/timidity		#Link to ubuntu midi playback binary, change this if you compiled it yourself

TCHROM := testchrom.cpp chromosome.cpp
TCHROMOBJ := $(addprefix obj/, $(TCHROM:.cpp=.o))

TMIDI := MIDI-output.cpp testmidi.cpp chromosome.cpp
TMIDIOBJ := $(addprefix obj/, $(TMIDI:.cpp=.o))

TFIT := testfit.cpp fitness.cpp chromosome.cpp
TFITOBJ := $(addprefix obj/, $(TFIT:.cpp=.o))

CC := g++
CFLAGS := -g -Wall
OBJECTS := $(addprefix obj/, $(SOURCEFILES:.cpp=.o))
EXECUTABLE := genetic

all: $(SOURCES) $(EXECUTABLE) $(HEADERS)

testchrom: $(TCHROMOBJ) $(HEADERS)
	$(CC) $(TCHROMOBJ) -o $@
	./$@

testmidi: $(TMIDIOBJ) $(HEADERS)
	$(CC) $(TMIDIOBJ) -o $@
	./$@
	$(PLAYBACK) $(TESTMIDI)

testfit: $(TFITOBJ) $(HEADERS)
	$(CC) $(TFITOBJ) -o $@
	./$@

$(EXECUTABLE): $(OBJECTS) $(HEADERS)
	$(CC) $(OBJECTS) -o $@
	./$@

obj/%.o: src/%.cpp src/%.h	#Force recompile if associated header changes
	$(CC) $(CFLAGS) -c $< -o $@

obj/%.o: src/%.cpp		#Overloaded object compiler for .cpp files without headers
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TCHROMOBJ) $(TMIDIOBJ) $(EXECUTABLE) $(TESTMIDI) $(TESTEXE) src/*~
