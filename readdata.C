#include "readdata.h"
#include <iostream>
#include "chambergeometry.h"
#include "defines.h"

readdata::readdata(const char* filnam) {
  fil.open(filnam, std::ios::in);
  isdone=false;
//   double testx, testy;
//   std::cout<<"In constructor\n";
//   getxy(testx, testy, 44, 1);
//   std::cout<<"testx "<<testx<<" testy "<<testy<<"\n";
//   getxy(testx, testy, 45, 1);
//   std::cout<<"testx "<<testx<<" testy "<<testy<<"\n";
//   getxy(testx, testy, 45, 2);
//   std::cout<<"testx "<<testx<<" testy "<<testy<<"\n";
//   getxy(testx, testy, 47, 3);
//   std::cout<<"testx "<<testx<<" testy "<<testy<<"\n";
//   getxy(testx, testy, 48, 3);
//   std::cout<<"testx "<<testx<<" testy "<<testy<<"\n";
//   getxy(testx, testy, 49, 3);
//   std::cout<<"testx "<<testx<<" testy "<<testy<<"\n";
//   getxy(testx, testy, 50, 3);
//   std::cout<<"testx "<<testx<<" testy "<<testy<<"\n";

}

void readdata::readline() {
  fil>>xl>>yl>>theta>>shift>>chi2>>ndof>>nh;
  if(!fil.good()) {
    isdone=true;
    nh=0;
  }
}

bool readdata::readhits() {
  hits.clear();
  hits.reserve(nh);
  bool isbigwheel=true;
  for(int i=0; i<nh; i++) {
    fil>>hits[i].station>>hits[i].multilayer>>hits[i].layer>>hits[i].tube>>hits[i].res>>hits[i].xh>>hits[i].yh>>hits[i].rh>>hits[i].drh>>hits[i].th>>hits[i].ri>>hits[i].ti;
    if(!fil.good()) isdone=true;
    if(hits[i].station!=5) isbigwheel=false;
  }
  char garb[35];
  fil>>garb;
  if(!fil.good()) isdone=true;
  return isbigwheel;
}

bool readdata::readsegment() {
  readline();
  return readhits();
}

void readdata::show() {
  double x,y;
  std::cout<<" xl "<<xl<<" yl "<<yl<<" theta "<<theta<<" shift "<<shift<<" chi2 "<<chi2<<" ndof "<<ndof<<" nh "<<nh<<"\n";
  for(int i=0; i<nh; i++) {
    getxy(x,y,  hits[i].tube, LAYERS_PER_ML * (hits[i].multilayer - 1) + hits[i].layer);
    std::cout<<"stat "<<hits[i].station
	     <<" ml "<<hits[i].multilayer
	     <<" l "<<hits[i].layer
	     <<" t "<<hits[i].tube
	     <<" res "<<hits[i].res
	     <<" xh "<<hits[i].xh<<" yh "<<hits[i].yh
	     <<" rh "<<hits[i].rh<<" drh "
	     <<hits[i].drh<<" th "<<hits[i].th<<" ri "<<hits[i].ri<<" ti "<<hits[i].ti
	     <<" testx "<<x<<" testy "<<y<<" "
	     <<" diffx "<<hits[i].xh - x<<" diffy "<<hits[i].yh-y
	     <<"\n";
  }
  std::cout<<"----------\n";
}

bool readdata::nextsegment(segment &seg, DCOnTrackVec& dchits) {
  bool isbw;
  dchits.clear();
  do {
    isbw = readsegment();
  } while(!(isbw || isdone));

  //  show();
  //  std::cout<<"ret "<<isdone<<"\n";
  
  if(isdone) return false;
  if(!isbw) std::cout<<"The world is dying.\n";

  seg.makesegment(xl, yl, theta);
  DCOnTrack tmp;

  for(int ii=0; ii<nh; ii++) {
    int i= hits[ii].tube;
    int j = LAYERS_PER_ML * (hits[ii].multilayer - 1) + hits[ii].layer;
    getxy(tmp.x, tmp.y, i, j);
    //tmp.x = hits[ii].xh;
    //tmp.y = hits[ii].yh;

    tmp.dr = 2;
    tmp.r = hits[ii].ri;
    tmp.t = hits[ii].ti;
    tmp.rtrue = hits[ii].rh;
    tmp.ttrue = hits[ii].th;
    dchits.push_back(tmp);
  }
  
  return true;
}
