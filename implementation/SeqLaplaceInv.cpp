#include "SeqLaplaceInv.h"

SeqLaplaceInv::SeqLaplaceInv(): LaplaceInv() {
}

SeqLaplaceInv::SeqLaplaceInv(complex<double> (*func)(double,double)): LaplaceInv(func) {
}

double SeqLaplaceInv::operator()(double t){
  //TODO
  return t;
}

