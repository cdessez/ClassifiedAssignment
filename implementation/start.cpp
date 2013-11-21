#include "start.h"

int main() {

  SeqLaplaceInv func(&L);
  Plotter plotter(func);
  vector<double> v;
  for(double t = -5.0; t <= 5.0; t+=0.1){
    v.push_back(t);
  }

  plotter.plot(v);

  return 0;
}
