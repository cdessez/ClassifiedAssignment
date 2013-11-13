CXX=g++
CXXFLAGS=

DEPS=start.h mystery.h
OBJ=start.o
LIBS=mystery.o
EXE=start

all: $(EXE)

%.o: %.cpp $(DEPS)
	$(CXX) -Wall -c -o $@ $< $(CXXFLAGS)

$(EXE): $(OBJ)
	$(CXX) -o $@ $< $(CXXFLAGS) $(LIBS)

clean:
	rm -f $(OBJ)
	rm -f $(EXE)
