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

class MPILaplaceInv : public LaplaceInv {

  public:
    MPILaplaceInv();
    MPILaplaceInv(complex<double> (*func)(double,double));
    MPILaplaceInv(complex<double> (*func)(double,double), int N, int M);
    double operator()(double t);
    vector<double> operator()(vector<double> &v);
  protected:
    int N;
    int M;
    void waitForConnectionSocket(int port);
  private:
};

#endif
