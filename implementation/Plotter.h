#ifndef PLOTTER_H
#define PLOTTER_H

#include <iostream>
#include <vector>

#include "LaplaceInv.h"

using namespace std;

class Plotter {
  
  public:
    Plotter(LaplaceInv &func);
    void plot(double tmin, double tmax, double tinc);
    void plot(vector<double> v);

  protected:
    LaplaceInv func;
  private:
};

#endif
