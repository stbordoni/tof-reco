# List all header files here
HDRS = TofHit.h TofSignal.h TofEvent.h TofRun.h

# List all source files here
SRCS = main.cpp TofHit.cpp TofSignal.cpp TofEvent.cpp TofRun.cpp TofObjectsDict.cpp

# List all object files here
OBJS = main.o TofHit.o TofSignal.o TofEvent.o TofRun.o TofObjectsDict.o

# Compiler flags
CFLAGS = -c -Wall -O2 -std=c++11 $(shell root-config --cflags)

# Linker flags
LFLAGS = $(shell root-config --ldflags)

# Libraries
LIBS = $(shell root-config --libs)

# Name of the executable
EXECUTABLE = my_program

# Default target
all: $(EXECUTABLE)

# Target to build the executable
$(EXECUTABLE): $(OBJS)
	$(CXX) $(LFLAGS) -o $@ $^ $(LIBS)

# Target to build object files
%.o: %.cpp $(HDRS)
	$(CXX) $(CFLAGS) -o $@ $<

# Target to generate dictionary information
TofObjectsDict.cpp: $(HDRS)
	rootcint -f $@ -c $(HDRS)

# Clean up
clean:
	rm -f $(OBJS) $(EXECUTABLE) TofObjectsDict.cpp TofObjectsDict.h
