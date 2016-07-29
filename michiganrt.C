#include "michiganrt.h"
#include<fstream>
#include<iostream>
#include<math.h>
#include "Rtypes.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Michigan RT functions
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::fstream readrt;

// use to simulate hits
double simul_r[100];
double simul_t[100];
double simul_dr[100];

double simul_min;
double simul_max;
int simul_count;

double rec_r[100];
double rec_t[100];
double rec_dr[100];

double rec_min;
double rec_max;
int rec_count;


void readin(char *fil, int type) {
  double r, t, dr;
  double rprev=-1e10, tprev=-1e10;
  int i=0;
  readrt.open(fil, std::ios::in);

  double *arr_r, *arr_t, *arr_dr;
  double *arr_min, *arr_max;
  int *arr_count;
  if(type==0) {
    arr_r = simul_r;
    arr_t = simul_t;
    arr_dr = simul_dr;
    arr_min = &simul_min;
    arr_max = &simul_max;
    arr_count = &simul_count;

  } else {
    arr_r = rec_r;
    arr_t = rec_t;
    arr_dr = rec_dr;
    arr_min = &rec_min;
    arr_max = &rec_max;
    arr_count = &rec_count;

  }
 
  while(1) {
    readrt>>r>>t>>dr;
    if(!readrt.good()) break;
    arr_r[i] = r;
    arr_t[i] = t;
    arr_dr[i] = dr;
    if(rprev>r || tprev > t) std::cout<<"This RT function is fucked up\n";
    rprev = r;
    tprev = t;
    i++;    
  }
  (*arr_count) = i;
  std::cout<<"Number of points: "<<(*arr_count)<<"\n";
  (*arr_min) = arr_t[0];
  (*arr_max) = arr_t[(*arr_count) - 1];  
  readrt.close();
}

int getsimbin(double t) {
  if(t<simul_min) return -1;
  if(t>=simul_max) return simul_count-1;
  int mid = (int) floor(simul_count*t/(simul_max - simul_min)); 
  while(simul_t[mid] > t ) mid--;
  return mid ;
}

double simrad(double t) {
  int bin = getsimbin(t);
  //  std::cout<<"in simrad bin is "<<bin<<"\n"; 
  if(bin<0) return simul_r[0];
  if(bin==(simul_count-1)) 
    return (simul_r[bin] - simul_r[bin-1])/(simul_t[bin] - simul_t[bin-1]) * (t - simul_t[bin] ) + simul_r[bin] ; 
  return (simul_r[bin+1] - simul_r[bin])/(simul_t[bin+1] - simul_t[bin]) * (t - simul_t[bin] ) + simul_r[bin] ; 
}

double TF1simrad(Double_t *t, Double_t *par) {
  return simrad(t[0]);
}

double simres(double t) {
  int bin = getsimbin(t);
  if(bin<0) return simul_dr[0];
  if(bin==(simul_count-1)) 
    return (simul_dr[bin] - simul_dr[bin-1])/(simul_t[bin] - simul_t[bin-1]) * (t - simul_t[bin] ) + simul_dr[bin] ; 
  return (simul_dr[bin+1] - simul_dr[bin])/(simul_t[bin+1] - simul_t[bin]) * (t - simul_t[bin] ) + simul_dr[bin] ; 
}

double simtime(double r) {
  double ta = simul_min;
  double tb = 786.7;
  if(r<simrad(ta) ) {
    return simul_min;
  } else if(r>simrad(tb)) {
    return simul_max;
  }
  
  int itr = 0;
  while (ta <= tb) {
    double tm  = (ta + tb) / 2;  // compute mid point.
    //    std::cout<<" ta is "<<ta<<" tb is "<<tb<<" tm is "<<tm<<std::flush;
    double rtm = simrad(tm);
    //    std::cout<<" rtm is "<<rtm<<"\n";

    if(fabs(rtm - r) < 0.001 ) return tm;
    else if (r > rtm) ta = tm;
    else if (r < rtm ) tb = tm;    
    itr++;
    if(itr>50) return -1;
  }
  return -1;    // failed to find key
}

int getrecbin(double t) {
  if(t<rec_min) return -1;
  if(t>=rec_max) return rec_count-1;
  int mid = (int) floor(rec_count*t/(rec_max - rec_min)); 
  while(rec_t[mid] > t ) mid--;
  return mid ;
}

double recrad(double t) {
  int bin = getrecbin(t);
  if(bin<0) return rec_r[0];
  if(bin==(rec_count-1)) 
    return (rec_r[bin] - rec_r[bin-1])/(rec_t[bin] - rec_t[bin-1]) * (t - rec_t[bin] ) + rec_r[bin] ; 
  return (rec_r[bin+1] - rec_r[bin])/(rec_t[bin+1] - rec_t[bin]) * (t - rec_t[bin] ) + rec_r[bin] ; 
}

double TF1recrad(Double_t *t, Double_t *par) {
  return recrad(t[0]);
}

double recres(double t) {
  int bin = getrecbin(t);
  if(bin<0) return rec_dr[0];
  if(bin==(rec_count-1)) 
    return (rec_dr[bin] - rec_dr[bin-1])/(rec_t[bin] - rec_t[bin-1]) * (t - rec_t[bin] ) + rec_dr[bin] ; 
  return (rec_dr[bin+1] - rec_dr[bin])/(rec_t[bin+1] - rec_t[bin]) * (t - rec_t[bin] ) + rec_dr[bin] ; 
}

double rectime(double r) {
  double ta = rec_min;
  double tb = 764.66;
  if(r<recrad(ta) ) {
    return rec_min;
  } else if(r>recrad(tb)) {
    return rec_max;
  }
  
  int itr = 0;
  while (ta <= tb) {
    double tm  = (ta + tb) / 2;  // compute mid point.
    double rtm = recrad(tm);
    if(fabs(rtm - r) < 0.001 ) return tm;
    else if (r > rtm) ta = tm;
    else if (r < rtm ) tb = tm;    
    itr++;
    if(itr>50) return -1;
  }
  return -1;    // failed to find key
}
