#ifndef LAPLACEINV_H
#define LAPLACEINV_H

#include <iostream>
#include <complex>
#include <vector>

using namespace std;

class LaplaceInv {
  // a virtual class to handle the different ways to process the calculus
  public:
    LaplaceInv();
    LaplaceInv(complex<double> (*func)(double,double));
    void setFunc(complex<double> (*func)(double,double));
    virtual double operator()(double t) = 0;
    virtual vector<double> operator()(vector<double> &v);
  protected:

  protected:
    complex<double> (*func)(double,double);
};

#endif
