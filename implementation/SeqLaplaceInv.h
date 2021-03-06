#ifndef SEQLAPLACEINV_H
#define SEQLAPLACEINV_H

#include <math.h>
#include <time.h>
#include <iomanip>
#include "LaplaceInv.h"
#include "mystery.h"

class SeqLaplaceInv : public LaplaceInv {

  public:
    SeqLaplaceInv();
    SeqLaplaceInv(complex<double> (*func)(double,double));
    SeqLaplaceInv(complex<double> (*func)(double,double), int N, int M);
    double operator()(double t);
    vector<double> operator()(vector<double> &v);
    void setDisplayTime(bool b);
  protected:
    int N;
    vector<int> Cm; // the binomial coefficients
    int sum_Cm;
    int M;
    bool displayTime; // whether or not the execution time is printed to cout

  private:
    inline int C(int k, int m);
};

#endif
