#ifndef SEGMENT_H
#define SEGMENT_H 1

class segment {
public:
  double x();
  double y();
  double theta();
  double getxat(double y);
  double getyat(double x);
  double residual(double xh, double yh, double rh);

  void makesegment(double x, double y, double theta);
  void makerandomsegment(double xmax, double ymax); 
  void makemidlinesegment(double xmax, double ymax); 
  double distancetopoint(double xp, double yp);  

private:  
  void computeparams();
  double m_x;
  double m_y;
  double m_theta;
  
  double A;
  double B;
  double C;
  double r_A2B2;
  double tantheta;
  double yintercept;
  double costheta;

} ;

#endif

