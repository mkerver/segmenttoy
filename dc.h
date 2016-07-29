#ifndef DC_H
#define DC_H 1

#include <vector>

struct DCOnTrack {
  double x;
  double y;
  double dr;
  double r;
  double t;
  double rtrue;
  double ttrue;
  //  double thetatrue;  
};

typedef std::vector<DCOnTrack> DCOnTrackVec;

void meanhitpos(std::vector<DCOnTrack> &hits, double &Zc, double &Yc) ;


#endif
