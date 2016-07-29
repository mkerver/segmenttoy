#include "rt.h"
#include <math.h>
#include<iostream>
#include "Rtypes.h"


/////////////////////////////////////// RT functions ///////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Craig Blocker RT functions
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


static double t2r_a[10] = {1.184169e-1, 3.32382e-2, 4.179808e-4, -5.012896e-6, 2.61497e-8, -7.800677e-11, 1.407393e-13, -1.516193e-16, 8.967997e-20, -2.238627e-23};
static double rcorr_a[] = {234.3413, -5.803375, 5.061677e-2, -1.994959e-4, 4.017433e-7, -3.975037e-10, 1.522393e-13};
  
double rcorr(double tin) {
  double rc;
  if(tin < 16.) {
    rc = -50.;
  } else {
    double tpow = 1.;
    rc = 0.;
    for(int j = 0; j < 7; j++) {
      rc += rcorr_a[j]*tpow;
      tpow *= tin;
    }
  }
  rc = rc/1000.;
    
  return rc;
}
  
double t2r(double tin) {
  if(tin < 0.) return 0.;
  if(tin > MAX_DRIFT) {
//    std::cout<<"tin "<<tin<<"\n"; 
    return 20.;
  }
    
  double tpow = 1.;
  double rc = 0.;
  for(int j = 0; j < 10; j++) {
    rc += t2r_a[j]*tpow;
    tpow *= tin;
  }
    
  rc -= rcorr(tin);
  return rc;
}

double TF1t2r(Double_t *t, Double_t *par) {
  return t2r(t[0]);
}  
/// derivatives of RT function, use to get errors
double rcorrprime(double tin) {
  double rc;
  if(tin < 16.) {
    rc = 0.;
  } else {
    double tpow = 1.;
    rc = 0.;
    for(int j = 1; j < 7; j++) {
      rc += j*rcorr_a[j]*tpow;
      tpow *= tin;
    }
  }
  rc = rc/1000.;
    
  return rc;
}
  
double t2rprime(double tin) {
  if(tin < 0.) return 0.;
  if(tin > MAX_DRIFT) return 20.;
  
  double tpow = 1.;
  double rc = 0.;
  for(int j = 1; j < 10; j++) {
    rc += j*t2r_a[j]*tpow;
    tpow *= tin;
  }
  
  rc -= rcorrprime(tin);
  return rc;
}

double r2t(double r) {
  double ta = 0;
  double tb = MAX_DRIFT;
  if(r<t2r(ta) ) {
    return 0;
  } else if(r>t2r(tb)) {
    return MAX_DRIFT;
  }
  
  int itr = 0;
  while (ta <= tb) {
    double tm  = (ta + tb) / 2;  // compute mid point.
    double rtm = t2r(tm);
    if(fabs(rtm - r) < 0.001 ) {
      return tm;
    }
    else if (r > rtm) {
      ta = tm;  // repeat search in top half.
    }
    else if (r < rtm ) {
      tb = tm; // repeat search in bottom half.
    }
    
    itr++;
    if(itr>50) return -1;
  }
  return -1;    // failed to find key
}

