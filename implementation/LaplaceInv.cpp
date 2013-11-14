#include "LaplaceInv.h"


LaplaceInv::LaplaceInv(): func(NULL){
}


LaplaceInv::LaplaceInv(complex<double> (*func)(double,double)): func(NULL){
}


void LaplaceInv::setFunc(complex<double> (*func)(double,double)){
  this->func = func;
}


vector<double> LaplaceInv::operator()(vector<double> &v){
  vector<double> res(v.size());
  for(size_t i = 0; i < v.size(); i++)
    res[i] = (*this)(v[i]);
  return res;
}
