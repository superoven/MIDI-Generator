SOURCEFILES := genetic.cpp chromosome.cpp MIDI-output.cpp fitness.cpp
SOURCES := $(addprefix src/, $(SOURCEFILES))
HEADERS := $(addprefix src/, chromosome.h genetic.h MIDI-output.h)
TESTMIDI := $(addprefix midi/, chromatic.mid notes.mid accents.mid)
TESTEXE := testchrom testmidi

TCHROM := testchrom.cpp chromosome.cpp
TCHROMOBJ := $(addprefix obj/, $(TCHROM:.cpp=.o))

TMIDI := MIDI-output.cpp testmidi.cpp chromosome.cpp
TMIDIOBJ := $(addprefix obj/, $(TMIDI:.cpp=.o))

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
	./bin/timidity $(TESTMIDI)

$(EXECUTABLE): $(OBJECTS) $(HEADERS)
	$(CC) $(OBJECTS) -o $@
	./$@

obj/%.o: src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TCHROMOBJ) $(TMIDIOBJ) $(EXECUTABLE) $(TESTMIDI) $(TESTEXE) src/*~
