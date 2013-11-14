#ifndef SEQLAPLACEINV_H
#define SEQLAPLACEINV_H

#include "LaplaceInv.h"

class SeqLaplaceInv : public LaplaceInv {

  public:
    virtual double operator()(double t);
  protected:

  private:
};

#endif
