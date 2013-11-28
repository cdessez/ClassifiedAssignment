#####
# MAIN PROGRAM
#####

CXX = g++
CXXFLAGS = 

DEPS = start.h mystery.h LaplaceInv.h SeqLaplaceInv.h MPILaplaceInv.h Plotter.h
OBJ = start.o LaplaceInv.o SeqLaplaceInv.o MPILaplaceInv.o Plotter.o
LIBS = mystery.o
EXE = whattime

#####
# MPI PROGRAM
#####

MPICXX = mpic++
MPICXXFLAGS = -fopenmp
MPIOBJ = mpicore.o LaplaceInv.o SeqLaplaceInv.o mpi_func.o
MPILIBS = mystery.o 
MPIEXE = mpicore

#####
# RULES
#####

all: $(EXE) $(MPIEXE)

%.o: %.cpp $(DEPS)
	$(CXX) -Wall -c -o $@ $< $(CXXFLAGS)

$(EXE): $(OBJ)
	$(CXX) -o $@ $(OBJ) $(CXXFLAGS) $(LIBS)
	chmod +x $@

mpicore.o: mpicore.cpp
	$(MPICXX) -Wall -c -o $@ $< $(MPICXXFLAGS)

mpi_func.o: mpi_func.cpp mpi_func.h
	$(MPICXX) -Wall -c -o $@ $< $(MPICXXFLAGS)

$(MPIEXE): $(MPIOBJ)
	$(MPICXX) -o $@ $(MPIOBJ) $(MPICXXFLAGS) $(MPILIBS)

clean:
	rm -f $(OBJ)
	rm -f $(EXE)
	rm -f $(MPIOBJ)
	rm -f $(MPIEXE)
	rm -f in.txt out.txt

