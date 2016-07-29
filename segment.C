#include "segment.h"
#include "random.h"
#include <math.h>

#define PI 3.14159265


double segment::x() { 
  return m_x;
}

double segment::y() { 
  return m_y; 
}

double segment::theta() { 
  return m_theta;
}

double segment::getxat(double y) {
  return (y-m_y)/tantheta + m_x;
}

double segment::getyat(double x) {
  return (x-m_x)*tantheta + m_y;
}

double segment::residual(double xh, double yh, double rh) {
  double yl = yh - tantheta*xh - yintercept;
  return fabs(yl * costheta) - rh;
}

void segment::computeparams() {
  A = tan(m_theta);
  B = -1;
  C = m_y - A*m_x;
  r_A2B2 = sqrt(A*A + B*B) ;
  tantheta = A;
  yintercept = getyat(0);
  costheta = cos(m_theta);
}

void segment::makesegment(double x, double y, double theta) {
  m_x = x;
  m_y = y;
  m_theta = theta ;
  computeparams();
}

void segment::makerandomsegment(double xmax, double ymax) {
  m_x = randDouble() * xmax;
  m_y = randDouble() * ymax;
  m_theta = randDouble() * PI;
  computeparams();  
}

void segment::makemidlinesegment(double xmax, double ymax) {
  m_x = randDouble() * xmax;
  m_y = ymax/2.0;
  m_theta = randDouble() * PI;
  computeparams();  
}

double segment::distancetopoint(double xp, double yp) {
  return fabs(A*xp + B*yp + C)/r_A2B2;
}

