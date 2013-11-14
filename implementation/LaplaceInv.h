#ifndef LAPLACEINV_H
#define LAPLACEINV_H

#include <iostream>
#include <complex>
#include <vector>

#include "mystery.h"

using namespace std;

class LaplaceInv {
  
  public:
    LaplaceInv();
    LaplaceInv(complex<double> (*func)(double,double));
    void setFunc(complex<double> (*func)(double,double));
    virtual double operator()(double t) = 0;
    virtual vector<double> operator()(vector<double> &v);
  protected:

  private:
    complex<double> (*func)(double,double);
};

#endif
