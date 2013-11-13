CXX = g++
CXXFLAGS =

DEPS = start.h mystery.h ToolBox.h
OBJ = start.o ToolBox.o
LIBS = mystery.o
EXE = start

all: $(EXE)
	if [ ! -d "outputdata" ]; then mkdir outputdata; fi

%.o: %.cpp $(DEPS)
	$(CXX) -Wall -c -o $@ $< $(CXXFLAGS)

$(EXE): $(OBJ)
	$(CXX) -o $@ $(OBJ) $(CXXFLAGS) $(LIBS)

clean:
	rm -f $(OBJ)
	rm -f $(EXE)

clean-outputdata:
	rm -f outputdata/*
