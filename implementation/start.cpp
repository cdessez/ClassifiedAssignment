#include "start.h"

int main() {

  SeqLaplaceInv func(&L);
  Plotter plotter(func);
  vector<double> v;
  for(double t = 0.1; t <= 12.0; t+=0.2){
    v.push_back(t);
  }

  plotter.plot(v);

  return 0;
}
