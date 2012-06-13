SOURCES := testchrom.cpp chromosome.cpp
HEADERS := chromosome.h

TCHROM := testchrom.cpp chromosome.cpp
TCHROMOBJ :=$(TCHROM:.cpp=.o)

CC := g++
CFLAGS := -g -Wall
OBJECTS :=$(SOURCES:.cpp=.o)
EXECUTABLE := genetic

all: $(SOURCES) $(EXECUTABLE)

testchrom: $(TCHROMOBJ) $(HEADERS)
	$(CC) $(TCHROMOBJ) -o $@

$(EXECUTABLE): $(OBJECTS) $(HEADERS)
	$(CC) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm *.o $(EXECUTABLE) testchrom
