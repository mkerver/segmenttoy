#ifndef DATAFILE_H
#define DATAFILE_H 1
#include "segment.h"
#include "dc.h"
#include <fstream>
#include <vector>

class readhit {
 public:
  int station;
  int multilayer; 
  int layer;
  int tube;
  double xh;
  double yh;
  double res;
  double rh;
  double drh;
  double th;
  double ri;
  double ti;
};

class readdata {
 public:
  readdata(const char*);
  bool nextsegment(segment &seg, DCOnTrackVec& dchits);

  double getshift() { return shift; }
  double getchi2() { return chi2; }
  int getndof() { return ndof; }
  int getnh() { return nh; }


 private:
  void readline();
  bool readhits();
  bool readsegment();
  void show();

  std::fstream fil;
  bool isdone;

  std::vector<readhit> hits;
  double xl;
  double yl;
  double theta;
  double shift;
  double chi2;
  int ndof;
  int nh;

};

#endif
