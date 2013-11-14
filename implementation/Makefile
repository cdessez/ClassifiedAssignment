CXX = g++
CXXFLAGS =

DEPS = start.h mystery.h LaplaceInv.h SeqLaplaceInv.h MPILaplaceInv.h Plotter.h
OBJ = start.o LaplaceInv.o SeqLaplaceInv.o MPILaplaceInv.o Plotter.o
LIBS = mystery.o
EXE = start

all: $(EXE)

%.o: %.cpp $(DEPS)
	$(CXX) -Wall -c -o $@ $< $(CXXFLAGS)

$(EXE): $(OBJ)
	$(CXX) -o $@ $(OBJ) $(CXXFLAGS) $(LIBS)

clean:
	rm -f $(OBJ)
	rm -f $(EXE)

