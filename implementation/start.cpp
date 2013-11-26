#include "start.h"

int main(int argc, char **argv) {
  
  LaplaceInv *func;

  if (argc == 2 && string(argv[1]).compare("--mpi") == 0)
    func = new MPILaplaceInv(&L);
  else
    func = new SeqLaplaceInv(&L);
  
  Plotter plotter(*func);
  vector<double> v;
  for(double t = 0.; t <= 12.01; t+=0.1){
    v.push_back(t);
  }

  plotter.plot(v);

  return 0;
}
