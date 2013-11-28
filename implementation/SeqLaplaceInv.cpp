#include "SeqLaplaceInv.h"

static double PI = atan(1)*4;
static int defaultN = 100;
static int defaultM = 15;
static double A = 18.4;
static double expAo2 = exp(A/2.);

SeqLaplaceInv::SeqLaplaceInv(): LaplaceInv(), N(defaultN), 
        M(defaultM), Cm(defaultM+1), sum_Cm(0x1 << defaultM), 
        displayTime(false) {
  for (int i = 0; i <= M; i++)
    Cm[i] = C(i,M);
}

SeqLaplaceInv::SeqLaplaceInv(complex<double> (*func)(double,double)): 
        LaplaceInv(func), N(defaultN), M(defaultM), Cm(defaultM+1), 
        sum_Cm(0x1 << defaultM), displayTime(false) {
  for (int i = 0; i <= M; i++)
    Cm[i] = C(i,M);
}

SeqLaplaceInv::SeqLaplaceInv(complex<double> (*func)(double,double), int N, 
        int M): LaplaceInv(func), N(N), M(M), Cm(M+1), sum_Cm(0x1 << M), 
        displayTime(false) {
  for (int i = 0; i < M; i++)
    Cm[i] = C(i,M);
}

double SeqLaplaceInv::operator()(double t){
  const double U = expAo2/t;
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

vector<double> SeqLaplaceInv::operator()(vector<double> &v){
  vector<double> res(v.size());
  double tbeg, tend;
  tbeg = time(0);
  for(int i = 0; i < v.size(); i++){
    res[i] = (*this)(v[i]);
  }
  tend = time(0);
  if (displayTime)
    cout << "Execution time: " << tend - tbeg << " s" << endl;
  return res;
}

void SeqLaplaceInv::setDisplayTime(bool b){
  displayTime = b;
}
