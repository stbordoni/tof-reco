# Paths
SRCDIR = src
INCDIR = include
BUILDDIR = build

# List all header files here
HDRS = TofHit.h TofSignal.h TofEvent.h TofRun.h

# List all source files here
SRCS = $(SRCDIR)/TofHit.cpp $(SRCDIR)/TofSignal.cpp $(SRCDIR)/TofEvent.cpp $(SRCDIR)/TofRun.cpp $(SRCDIR)/TofObjectsDict.cpp

# List all object files here
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SRCS)) main.o

# Compiler flags
CFLAGS = -c -Wall -O2 -std=c++11 -pthread -std=c++17 -m64 -I$(INCDIR) $(shell root-config --cflags)

# Linker flags
LFLAGS = $(shell root-config --ldflags)

# Libraries
LIBS = $(shell root-config --libs)

# Name of the executable
EXECUTABLE = main

# Default target
all: $(EXECUTABLE)

# Target to build the executable
$(EXECUTABLE): $(OBJS)
	$(CXX) $(LFLAGS) -o $@ $^ $(LIBS)

# Target to build object files
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp | $(BUILDDIR)
	$(CXX) $(CFLAGS) -o $@ $<

$(BUILDDIR)/main.o: main.cpp $(addprefix $(INCDIR)/,$(HDRS)) | $(BUILDDIR)
	$(CXX) $(CFLAGS) -o $@ $<

# Target to generate dictionary information
$(SRCDIR)/TofObjectsDict.cpp: $(addprefix $(INCDIR)/,$(HDRS))
	rootcint -f $@ -c -I$(INCDIR) $(addprefix ../,$(addprefix $(INCDIR)/,$(notdir $(HDRS))))

# Create build directory if it doesn't exist
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

# Clean up
clean:
	rm -f $(OBJS) $(EXECUTABLE) $(SRCDIR)/TofObjectsDict.cpp $(SRCDIR)/TofObjectsDict.h
	# rmdir $(BUILDDIR)
