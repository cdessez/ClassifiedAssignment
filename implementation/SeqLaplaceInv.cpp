#include "SeqLaplaceInv.h"

static double PI = atan(1)*4;
static int defaultN = 15;
static int defaultM = 12;

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
  const double A = 18.4;
	const double U = exp(A/2.)/t;
	const double X = A/(2.*t);
	const double H = PI/t;
	double Sum = func(X,0.).real()/2.;
  std::vector<double> CM;
  CM.push_back(1.);
  CM.push_back(11.);
  CM.push_back(55.);
  CM.push_back(165.);
  CM.push_back(330.);
  CM.push_back(462.);
  CM.push_back(462.);
  CM.push_back(330.);
  CM.push_back(165.);
  CM.push_back(55.);
  CM.push_back(11.);
  CM.push_back(1.);
	for(int i = 1; i <= N; i++) {
		double Y = i*H;
		Sum += (i % 2 == 0 ? func(X,Y).real() : -func(X,Y).real());
	}

	std::vector<double> SU;
	SU.push_back(Sum);

  for(int k = 1; k <= M; k++) {
		int i = N + k;
		double Y = i*H;
    double nextSU = SU.back() + (i % 2 == 0 ? func(X,Y).real() : -func(X,Y).real());
		SU.push_back(nextSU);
	}
  
	double Avgsu = 0;
	double Avgsu1 = 0;
	for(int j = 1; j <= M; j++) {
		Avgsu += CM[j-1]*SU[j-1];
		Avgsu1 += CM[j-1]*SU[j];
	}

	double sum_of_elems = 0;
	for(vector<double>::iterator j=CM.begin(); j!=CM.end(); ++j)
    	sum_of_elems += *j;

	double Fun = U*Avgsu/sum_of_elems;
	double Fun1 = U*Avgsu1/sum_of_elems;

	return Fun1;
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


