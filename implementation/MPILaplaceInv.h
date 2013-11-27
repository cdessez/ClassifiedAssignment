#ifndef MPILAPLACEINV_H
#define MPILAPLACEINV_H

#include "LaplaceInv.h"

#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>
#include <string>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>

enum ParallelType {
  MPI1 = 0,
  MPI2 = 1,
  MPI1_OPENMP = 2,
  MPI2_OPENMP = 3
};

class MPILaplaceInv : public LaplaceInv {

  public:
    MPILaplaceInv(int ptype);
    MPILaplaceInv(int ptype, int N, int M);
    double operator()(double t);
    vector<double> operator()(vector<double> &v);
  protected:
    int N;
    int M;
    int ptype;
    void waitForConnectionSocket(int port);
  private:
};

#endif
