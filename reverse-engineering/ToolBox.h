#ifndef TOOLBOX_H
#define TOOLBOX_H

#include <iostream>
#include <complex>
#include <cmath>
#include <fstream>

#include "mystery.h"

using namespace std;

class ToolBox {

  public:
    ToolBox();
    static bool grid(double xmin, double xmax, double xinc, double ymin, 
        double ymax, double yinc, string path);
    static bool circle(double xcenter, double ycenter, double radius, int npts,
        string path);
    static bool segment(double xbeg, double ybeg, double xend, double yend,
        int npts, string path);
    static bool sampleTests();

  protected:
    static void printPointToFile(fstream &f, double x, double y, complex<double> l);
  
  private:

};

#endif
