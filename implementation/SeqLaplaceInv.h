#ifndef SEQLAPLACEINV_H
#define SEQLAPLACEINV_H

#include "LaplaceInv.h"

class SeqLaplaceInv : public LaplaceInv {

  public:
    SeqLaplaceInv();
    SeqLaplaceInv(complex<double> (*func)(double,double));
    double operator()(double t);
  protected:

  private:
};

#endif
