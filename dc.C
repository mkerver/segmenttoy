#include "dc.h"

void meanhitpos(std::vector<DCOnTrack> &hits, double &Zc, double &Yc) {
  DCOnTrackVec::const_iterator it = hits.begin();
  DCOnTrackVec::const_iterator it_end = hits.end();
  int ii=0;
  double Sy, Sz, S;
  for( ; it != it_end; ++ii, ++it ) {
    double w = 1./hits[ii].dr;
    if(w<0) w = 0.;
    w*=w;
    S += w;
    Sz += w*hits[ii].x;
    Sy += w*hits[ii].y;
  }
  Zc = Sz/S;
  Yc = Sy/S;
}
