#include "Plotter.h"
#include "gnuplot_i.hpp"


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

  Gnuplot g1("f(t)");
  g1.set_style("lines").plot_xy(v,out,"Laplace Inversion");

  wait_for_key(); 
  //TODO use gnu plot to print it on the screen
}


void Plotter::wait_for_key(){
  cout << endl << "Press ENTER to continue" << endl;
  
  cin.clear();
  cin.ignore(cin.rdbuf()->in_avail());
  cin.get();
}
