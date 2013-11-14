#ifndef MPILAPLACEINV_H
#define MPILAPLACEINV_H

#include "LaplaceInv.h"

class MPILaplaceInv : public LaplaceInv {

  public:
    virtual double operator()(double t);
  protected:

  private:
};

#endif
