#ifndef FIT_H
#define FIT_H 1

#include "dc.h"

typedef std::vector<DCOnTrack> DCOnTrackVec;

class fitinfo {
 public:
  double chi2;
  double t0shift;
  double t0shifterr;
  int retval;
  int fcncount;
  int hitcount;
  int do1D;
  int dointernal;
  double d;
  double Zc;
  double Yc;
};

extern fitinfo idiocy;

bool fit( double xli, double yli, double thet, DCOnTrackVec *dchits, double &xf, double &yf, double &thetaf, DCOnTrackVec *dchits_new) ;

#endif
