#include "pattern.h"
#include <math.h>
#include <iostream>

#define MAXP 1965
#define PBINS 100

#define THETABINS 100

int theta[THETABINS];
int vert;

int bin(double angle) {
  int b = floor(THETABINS*angle/3.14159265);
  if(b<0 ) {
    std::cout<<"bin is negative\n";
    b=0;
  } else if(b>THETABINS) {
    std::cout<<"bin is too large\n";
    b=THETABINS-1;
  } else if(b==THETABINS) {
    std::cout<<"bin ceiling\n";
    b=THETABINS-1;
  }
  return b;
}

double angle(int bin) {
  return (bin + 0.5) * 3.14159265 / THETABINS ; 
}
void patternangle(const DCOnTrackVec& vec) {
  for(int i=0; i<100; i++) theta[i]=0;
  vert=0;
  int num = vec.size();
  double dx, dy, slope, ang;
  for(int i=0; i<num; i++) {
    //    for(int th=0; th<THETABINS; th++) {
    //  for(int pb=0; pb<200; pb++) 
    for(int j=i+1; j<num; j++) {
      if(vec[i].y > vec[j].y) {
	dx = vec[i].x - vec[j].x;
	dy = vec[i].y - vec[j].y;
      } else {
	dx = vec[j].x - vec[i].x;
	dy = vec[j].y - vec[i].y;
      }
      if(dx==0) {
	vert++;
	ang = 1.570796325;
      } else {
	ang = atan2(dy, dx);
      }
      theta[bin(ang)]++;
    }
  }
  int maxbin = 0;
  for(int i=0; i<100; i++) {
    if(theta[i]>theta[maxbin]) maxbin = i;
  }
  std::cout<<"max bin "<<maxbin;
  std::cout<<" theta "<<angle(maxbin);
  std::cout<<" vert "<<vert<<"\n";
}
