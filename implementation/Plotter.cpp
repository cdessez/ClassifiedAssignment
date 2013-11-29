#include "Plotter.h"
#include "gnuplot_i.hpp"

static const double PI = atan(1) * 4;


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
  
  // Plot the function
  Gnuplot g1("f(t)");
  g1.set_style("lines").plot_xy(v,out,"Inverse Laplace transform");

  // Plot a polarish clock like version of the output (just for fun)
  vector<double> x, y, cx, cy;
  for(int i = 0; i < v.size(); i++){
    double theta, r;
    theta = PI *(0.5 - v[i] / 6.);
    r = abs(out[i]);
    x.push_back(r * cos(theta));
    y.push_back(r * sin(theta));
    cx.push_back(6.5 * cos(theta));
    cy.push_back(6.5 * sin(theta));
  }
  Gnuplot g2("Clock");
  g2.set_xrange(-10,10).set_yrange(-7,7);
  g2.set_style("lines").plot_xy(x,y,
        "Clock View of the inverse Laplace transform");
  g2.plot_xy(cx,cy,"Frame");

  wait_for_key(); 
}


void Plotter::wait_for_key(){
  cout << endl << "Press CTRL+C to exit" << endl;
  
  cin.clear();
  cin.ignore(cin.rdbuf()->in_avail());
  while (true)
    cin.get();
}
