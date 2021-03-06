#include "LaplaceInv.h"


LaplaceInv::LaplaceInv(): func(NULL){
}


LaplaceInv::LaplaceInv(complex<double> (*func)(double,double)): func(func){
}


void LaplaceInv::setFunc(complex<double> (*func)(double,double)){
  this->func = func;
}


vector<double> LaplaceInv::operator()(vector<double> &v){
  // simply iterate the previous method over the values in v
  // This method may be overload if computing several values at one time is 
  // faster than simply calling several times the method for a unique value
  vector<double> res(v.size());
  for(size_t i = 0; i < v.size(); i++)
    res[i] = (*this)(v[i]);
  return res;
}
