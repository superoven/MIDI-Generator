SOURCES := genetic.cpp chromosome.cpp MIDI-output.cpp fitness.cpp
HEADERS := $(addprefix src/, chromosome.h genetic.h MIDI-output.h)

TCHROM := testchrom.cpp chromosome.cpp
TCHROMOBJ := $(addprefix obj/, $(TCHROM:.cpp=.o))

TMIDI := MIDI-output.cpp testmidi.cpp chromosome.cpp
TMIDIOBJ := $(addprefix obj/, $(TMIDI:.cpp=.o))

CC := g++
CFLAGS := -g -Wall
OBJECTS := $(addprefix obj/, $(SOURCES:.cpp=.o))
EXECUTABLE := genetic

all: $(SOURCES) $(EXECUTABLE)

testchrom: $(TCHROMOBJ) $(HEADERS)
	$(CC) $(TCHROMOBJ) -o $@

testmidi: $(TMIDIOBJ) $(HEADERS)
	$(CC) $(TMIDIOBJ) -o $@

$(EXECUTABLE): $(OBJECTS) $(HEADERS)
	$(CC) $(OBJECTS) -o $@

obj/%.o: src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f obj/*.o $(EXECUTABLE) testchrom testmidi src/*~
