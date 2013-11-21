#include "SeqLaplaceInv.h"

static int defaultN = 50;
static int defaultM = 15;

SeqLaplaceInv::SeqLaplaceInv(): LaplaceInv(), N(defaultN), 
        M(defaultM), Cm(defaultM) {
  for (int i = 0; i < M; i++)
    Cm[i] = C(i,M);
}

SeqLaplaceInv::SeqLaplaceInv(complex<double> (*func)(double,double)): 
        LaplaceInv(func), N(defaultN), M(defaultM), Cm(defaultM) {
  for (int i = 0; i < M; i++)
    Cm[i] = C(i,M);
}

SeqLaplaceInv::SeqLaplaceInv(complex<double> (*func)(double,double), int N, 
                              int M): 
        LaplaceInv(func), N(N), M(M), Cm(M) {
  for (int i = 0; i < M; i++)
    Cm[i] = C(i,M);
}

double SeqLaplaceInv::operator()(double t){
  //TODO
  return t;
}

int SeqLaplaceInv::C(int k, int m){
  long res = 1; 
  const long mk = m - k;
  for (int i = m; i > k; i--)
    res *= i;
  for (int i = 1; i <= mk; i++)
    res /= i;
  return static_cast<int>(res);
}


