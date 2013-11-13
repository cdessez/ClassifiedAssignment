#include "start.h"

using namespace std;

void printToF(fstream &f, double x, double y, complex<double> l){
  f << x << ',' << y << ',' << l.real() << ',' << l.imag() << ',' << norm(l)
    << ',' << arg(l) << endl;
}

int main() {
  
  fstream f;
  f.open("output/data.csv", fstream::out);

  for(double i = -10.0; i <= 10.0; i+= 0.1){
    for(double j = -10.0; j <= 10.0; j += 0.1){
      printToF(f, i, j, L(i, j));
    }
  }

  cout << "Values sent to data.csv";

  return 0;
}
