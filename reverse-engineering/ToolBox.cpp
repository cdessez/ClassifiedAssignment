#include "ToolBox.h"


ToolBox::ToolBox(){

}


bool ToolBox::grid(double xmin, double xmax, double xinc, double ymin,
    double ymax, double yinc, string path = "outputdata/grid.csv"){

  fstream f;
  f.open(path.c_str(), fstream::out);
  
  if (xmin > xmax || xinc < 0 || ymin > ymax || yinc < 0)
    return false;

  for(double x = xmin; x <= xmax; x+= xinc){
    for(double y = ymin; y <= ymax; y += yinc){
      printPointToFile(f, x, y, L(x, y));
    }
  }

  f.close();

  cout << "Process data for a grid, output in \"" << path << "\"" << endl;
  return true;
}


bool ToolBox::circle(double xcenter, double ycenter, double radius, 
    int npts, string path = "outputdata/circle.csv"){
  
  fstream f;
  f.open(path.c_str(), fstream::out);
  
  if (radius <= 0 || npts <= 0)
    return false;

  double anglestep = atan(1)*8 / npts;
  for(int i = 0; i < npts; i++){
    complex<double> c = complex<double>(xcenter, ycenter)
      + polar(radius, i * anglestep);
    printPointToFile(f, c.real(), c.imag(), L(c.real(), c.imag()));
  }

  f.close();

  cout << "Process data for a circle, output in \"" << path << "\"" << endl;
  return true;
}


bool ToolBox::segment(double xbeg, double ybeg, double xend, double yend,
    int npts, string path = "outputdata/segment.csv"){
  
  fstream f;
  f.open(path.c_str(), fstream::out);
  
  if (npts <= 0)
    return false;
  
  complex<double> beg(xbeg, ybeg), end(xend, yend);
  complex<double> step = (end - beg)* (1.0 / npts);
  complex<double> c = beg;
  for(int i = 0; i < npts; i++){
    c += step;
    printPointToFile(f, c.real(), c.imag(), L(c.real(), c.imag()));
  }

  f.close();

  cout << "Process data for a circle, output in \"" << path << "\"" << endl;
  return true;
  return false;
}


void ToolBox::printPointToFile(fstream &f, double x, double y, 
    complex<double> l){
  
  // Format the file as following:
  // x | y | real part of l | imag part of l | norm of l | argument (angle) of l
  f << x << ',' << y << ',' << l.real() << ',' << l.imag() << ',' << abs(l)
    << ',' << arg(l) << endl;
}


bool ToolBox::sampleTests(){

  return ToolBox::grid(-10.0, 10.0, 0.1, -10.0, 10.0, 0.25) &&
    ToolBox::circle(0.0, 0.0, 1.0, 120, "outputdata/circle_1.csv") &&
    ToolBox::circle(0.0, 0.0, 2.0, 120, "outputdata/circle_2.csv") &&
    ToolBox::circle(0.0, 0.0, 3.0, 120, "outputdata/circle_3.csv") &&
    ToolBox::circle(0.0, 0.0, 5.0, 120, "outputdata/circle_5.csv") &&
    ToolBox::segment(-1.0, 0.0, 1.0, 0.0, 50, "outputdata/segment_hori.csv") &&
    ToolBox::segment(0.0, -1.0, 0.0, 1.0, 50, "outputdata/segment_vert.csv") &&
    ToolBox::segment(-1.0, -1.0, 1.0, 1.0, 70, "outputdata/segment_diag.csv") &&
    ToolBox::segment(-1.0, 1.0, 1.0, -1.0, 70, "outputdata/segment_diag2.csv");
}


