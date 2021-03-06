#include "start.h"

void printHelp(){
  cout << "Usage: whattime TYPE [OPTION]" << endl << endl;
  cout << "where  TYPE := {--seq | --mpi MPITYPE}" << endl;
  cout << "       MPITYPE := { 1 | 2 }" << endl;
  cout << "       OPTION := { --openmp }" << endl << endl;
  cout << "NB:" << endl;
  cout << "* --seq combined with --openmp is equivalent to --seq only" << endl;
  cout << "* MPITYPE 1 corresponds to the parallelization of the calculus of";
  cout << " a single value (parallel version the algorithm)" << endl;
  cout << "* MPITYPE 2 corresponds to the distribution of the values";
  cout << "over multiple processes (chunked method)" << endl;
  cout << "* --openmp uses multithreading inside each mpi process ";
  cout << "(only type 2)" << endl;
  cout << endl << "/!\\ this process should be launched on the machine";
  cout << " pointed by the first line of the file \"hosts\"" << endl;
}

int main(int argc, char **argv) {
  
  LaplaceInv *func = NULL;
  
  // parses the arguments
  if (argc == 1){
    printHelp();
    return 0;
  } else if (argc >= 2){ 
    int ptype = MPI1;
    for (int i = 1; i < argc; i++){
      if (string(argv[i]).compare("--seq") == 0){
        func = new SeqLaplaceInv(&L);
        ((SeqLaplaceInv *) func)->setDisplayTime(true);
        break;
      } else if (string(argv[i]).compare("--mpi") == 0){
        if (i == argc - 1){
          printHelp();
          return 0;
        }
        if (string(argv[++i]).compare("1") == 0){
        } else if (string(argv[i]).compare("2") == 0){
          ptype += MPI2;
        } else {
          printHelp();
          return 0;
        }
      } else if (string(argv[i]).compare("--openmp") == 0){
        ptype += 2;
      }
    }
    if (ptype == MPI1_OPENMP)
      ptype = MPI1;
    if (func == NULL)
      func = new MPILaplaceInv(ptype);
  }  
  
  // Process and plots the result
  Plotter plotter(*func);
  vector<double> v;
  for(double t = 0.; t <= 12.01; t+=0.1){
    v.push_back(t);
  }
  plotter.plot(v);

  return 0;
}
