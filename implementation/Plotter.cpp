#include "Plotter.h"


Plotter::Plotter(LaplaceInv &func): func(func){
}


void Plotter::plot(double tmin, double tmax, double tinc){
  // a simple wrapper for plot(vector<double> v)
  if (tmin > tmax){
    cout << "Error in Plotter::plot(tmin,tmax,tinc): tmin > tmax" << endl;
    return;
  }
  if (tinc <= 0){
    cout << "Error in Plotter::plot(): tinc <= 0" << endl;
    return;
  }
  
  vector<double> v((tmax - tmin) / tinc);
  this->plot(v);
}


void Plotter::plot(vector<double> v){
  
  vector<double> out = func(v);
  //TODO use gnu plot to print it on the screen
}
