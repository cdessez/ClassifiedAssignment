#include "SeqLaplaceInv.h"

static double PI = atan(1)*4;
static int defaultN = 100;
static int defaultM = 15;

SeqLaplaceInv::SeqLaplaceInv(): LaplaceInv(), N(defaultN), 
        M(defaultM), Cm(defaultM+1), sum_Cm(0x1 << defaultM) {
  for (int i = 0; i <= M; i++)
    Cm[i] = C(i,M);
}

SeqLaplaceInv::SeqLaplaceInv(complex<double> (*func)(double,double)): 
        LaplaceInv(func), N(defaultN), M(defaultM), Cm(defaultM+1), 
        sum_Cm(0x1 << defaultM) {
  for (int i = 0; i <= M; i++)
    Cm[i] = C(i,M);
}

SeqLaplaceInv::SeqLaplaceInv(complex<double> (*func)(double,double), int N, 
        int M): LaplaceInv(func), N(N), M(M), Cm(M+1), sum_Cm(0x1 << M) {
  for (int i = 0; i < M; i++)
    Cm[i] = C(i,M);
}

double SeqLaplaceInv::operator()(double t){
  const double A = 18.4;
  const double U = exp(A/2.)/t;
  const double X = A/(2.*t);
  const double H = PI/t;
  double Sum = func(X,0.).real()/2.;
  
  double Y = 0.;
  for(int i = 1; i <= N; i++) {
    Y += H;
    Sum += ((i & 0x1) == 0x0 ? func(X,Y).real() : -func(X,Y).real());
  }

  double SU = Sum;
  double Avgsu = 0;
  int i = N;
  Y = i*H;
  for(int k = 0; k <= M; k++) {
    i++;
    Y += H;
    SU += (i & 0x1) == 0x0 ? func(X,Y).real() : -func(X,Y).real();
    Avgsu += static_cast<double>(Cm[k]) * SU;
  }
  
  return U * Avgsu / static_cast<double>(sum_Cm);
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


