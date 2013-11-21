#ifndef SEQLAPLACEINV_H
#define SEQLAPLACEINV_H

#include "LaplaceInv.h"

class SeqLaplaceInv : public LaplaceInv {

  public:
    SeqLaplaceInv();
    SeqLaplaceInv(complex<double> (*func)(double,double));
    SeqLaplaceInv(complex<double> (*func)(double,double), int N, int M);
    double operator()(double t);
  protected:
    int N;
    vector<int> Cm;
    int M;

  private:
    inline int C(int k, int m);
};

#endif
