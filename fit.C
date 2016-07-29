#include "TROOT.h"
#include "TMath.h"
#include "TVirtualFitter.h"
#include "TMinuit.h"

#include "fit.h"
//#include "dc.h"
#include "michiganrt.h"
#include "rt.h"

#include "Line.h"

#include <iostream>

// garbage, not actually used any more
#define MAX_DRIFT 855
#define R2TSPURIOUS 50000


// time corresponding to r=15 mm for internal rt
#define TUBE_TIME 757.22

#define WEAK_TOPO_T0ERROR 10
#define STRONG_TOPO_T0ERROR 50

// prints a message if a radius is bigger than this
#define MAX_RAD 16

fitinfo idiocy;

const int m_minHits = 5;
const bool m_dumpToFile = false;
const bool m_trace = false;
const bool m_dumpNoFit = false;
const bool m_constrainShifts = false;
const bool m_scaleErrors = true;
const bool use_shift_constraint = m_constrainShifts;

TMinuit *m_minuit = new TMinuit(3);

/////////////////////////////     Fit function and data ////////////////////////////////////////////
typedef struct {
  double z;
  double t;
  double y;
  double w;
  double r;
} hitcoords;
  
hitcoords *data;
int datasize;

double t_lo, t_hi;
int t0Error;


/// the fit function
/// gets distance between the line and the hit (in the frame whose direction is given by the line segment
/// and position is given by the weighted average of all the hits), subtracts off the radius from the rt relation etc 
void mdtSegmentT0Fcn(Int_t &npar, Double_t* grad, Double_t &fval, Double_t* par, Int_t jflag) {
  if(jflag < 0 || npar == 97) grad[0] = 0.;
  
  double ang = par[0];  
  double b = par[1];
  double t0 = par[2];
  
  double cosin = cos(ang);
  double sinus = sin(ang);
  
  idiocy.fcncount++;

  fval = 0.;
  // Add 50 nsec constraint 
  if(use_shift_constraint || t0Error == WEAK_TOPO_T0ERROR ) fval += par[2]*par[2]/(1.0 *t0Error*t0Error);
  
  double t, r, z, y, w, dist;
  for(int i = 0; i < datasize ; i++) {
    t = data[i].t - t0;
    z = data[i].z;
    y = data[i].y;
    w = data[i].w;
    dist = fabs(b*cosin + z*sinus - y*cosin); // same thing as fabs(a*z - y + b)/sqrt(1. + a*a);
   
    bool outsidewindow = false; 
    double uppercut;
    uppercut = TUBE_TIME;
    if(idiocy.dointernal) {
      if(t<0) r=0;
      else r = t2r(t);
    } else {
      if(t<0) r=0;
      else r = recrad(t);
    }

    if (t> uppercut ) {
      fval += (t-uppercut)* (t-uppercut)/100.;
      outsidewindow = true; 
    } else if (t < 0 ) {
      fval += (t*t)/100.;
      outsidewindow = true;
    } 
    
    if (outsidewindow) continue;
    fval += (dist - r)*(dist - r)*w;
  }
  
  return;
}

int sign(double a) {
  if(a>0) return 1;
  if(a<0) return -1;
  return 0;
}

bool fit( double xli, double yli, double thet, DCOnTrackVec *dchits, double &xf, double &yf, double &thetaf, DCOnTrackVec *dchits_new) {
  Line line( LocPos( xli, yli), thet );
  int N = (*dchits).size();

  idiocy.fcncount=0;
  idiocy.retval = -1;
  idiocy.chi2 = -1;
  idiocy.t0shift = -100000;
  idiocy.hitcount = N;
	
  if(N<2) return false;
  if(N < m_minHits) {
    if(m_trace) std::cout << "FEWER THAN FIVE HITS N "<<N<<"\n";
    return false; 
  }

  double Zc=0, Yc=0, S=0, Sz=0, Sy=0;
  std::vector<double> y(N);
  std::vector<double> z(N);
  std::vector<double> w(N); 
  std::vector<double> r(N);
  std::vector<double> t(N);

  // allocate memory for data
  data = new hitcoords[N];
  {
    for(int ii=0 ;ii<N; ++ii ) {
      if(m_trace) std::cout << "hit # "<<ii<<" ";
      y[ii] = (*dchits)[ii].y;
      z[ii] = (*dchits)[ii].x;
      r[ii] = (*dchits)[ii].r;
      t[ii] = (*dchits)[ii].t;

      w[ii] = 1./(*dchits)[ii].dr ;
      if(w[ii]<0) w[ii] = 0.;
      w[ii]*=w[ii];

      if(r[ii]<0){
        r[ii] = 0.;
        if(m_trace) std::cout << "MdtSegmentT0Fitter (using times) ERROR: <Negative r> " << r[ii]<< "\n";
      }
      if(m_trace) std::cout << "DC:  (" << y[ii] << "," << z[ii] << ") R = " << r[ii] << " W " << w[ii] <<" t " <<t[ii]<< "\n";

      S+=w[ii];
      Sz+= w[ii]*z[ii];
      Sy+= w[ii]*y[ii];
      if(r[ii] > MAX_RAD ) {
        if(m_trace) std::cout <<"Radius is too big\n";
      }
    }
  }
  Zc = Sz/S;
  Yc = Sy/S;
  for(int i=0;i<N;++i) {
    y[i]  -= Yc;
    z[i]  -= Zc;
  }
  
  DCOnTrackVec::const_iterator it = (*dchits).begin();
  DCOnTrackVec::const_iterator it_end = (*dchits).end();
  
  t_lo = 1e10;
  t_hi = -1e10;

  double t0seed=0; // the average t0 of the hit
  double st0 = 0; // the std deviation of the hit t0s
  double min_t0 = 1e10; // the smallest t0 seen
  double tee0;
  
  for(int ii=0 ;it!=it_end; ++it, ++ii ){
    data[ii].z = z[ii];
    data[ii].y = y[ii];
    data[ii].r = r[ii];
    data[ii].w = w[ii];

    double r2tval;
    if(idiocy.dointernal) {
      r2tval = r2t(r[ii]);
    } else {
      r2tval = rectime(r[ii]);
    }
    tee0 = t[ii] - r2tval;

    t0seed += tee0;
    st0 += tee0*tee0;
    if(tee0 < min_t0 && fabs(r2tval) < R2TSPURIOUS) min_t0 = tee0;
    
    data[ii].t = t[ii];
    if(t[ii]< t_lo) t_lo = t[ii];
    if(t[ii] > t_hi) t_hi = t[ii];
  }
  datasize = N;
  
  t0seed /= N;
  st0 = sqrt(st0/N - t0seed*t0seed);
  t_hi -= MAX_DRIFT;

  // ************************* seed the parameters
  double theta = line.phi();
  double cosin = cos(theta);
  double sinus = sin(theta);
    
  if ( sinus < 0.0 ) {
    sinus = -sinus;
    cosin = -cosin;
  } else if ( sinus == 0.0 && cosin < 0.0 ) {
    cosin = -cosin;
  }

  double d = line.y0() + Zc*sinus-Yc*cosin;
  
  // Calculate signed radii 
  int npos = 0;
  int nneg = 0;    
  double sdist;
  it = (*dchits).begin();
  for(int ii=0 ;it!=it_end; ++it, ++ii ){
    sdist = d*cosin + z[ii]*sinus - y[ii]*cosin; // same thing as fabs(a*z - y + b)/sqrt(1. + a*a);
    if (r[ii] < 1) continue;
    if(sdist > 0 ) npos++; 
    if(sdist < 0 ) nneg++; 
  }

  // Define t0 constraint in Minuit 
  t0Error = STRONG_TOPO_T0ERROR;
  if (npos < 2 || nneg < 2) t0Error = WEAK_TOPO_T0ERROR;

  if(m_trace) std::cout <<" positive radii " <<  npos << " negative radii " << nneg << " t0 Error " << t0Error  << "\n";
  
  m_minuit->SetFCN(mdtSegmentT0Fcn);
  Double_t arglist[3];
  
  // Set printout level
  arglist[0] = -1;
  m_minuit->SetPrintLevel(-1); // -1: no output, 1: std output
  // Clear previous fit
  m_minuit->mncler();
  arglist[0] = -1;
  
  Double_t vstart[3];
  vstart[0] = theta;
  // x' = x - xc, y' = y - yc => y' = m x' + b + m xc - yc
  // and b = yc - m xc
  vstart[1] = d ; 
  vstart[2] = 0 ;
  Double_t step[3] = {0.01 , 0.01 , 0.1 };
  
  if(m_trace) {
    std::cout <<"\n____________INITIAL VALUES________________\n";
    std::cout <<"Theta " << theta << "("<<tan(theta)<<") d " << vstart[1] <<"\n";
  }
    
  int errFlag = 0;
  m_minuit->mnparm(0, "a", vstart[0], step[0], -0.5, 3.5,errFlag);
  m_minuit->mnparm(1, "b", vstart[1], step[1], 0., 0.,errFlag);
  // m_minuit->SetParameter(2, "t0", vstart[2], step[2], t_hi, t_lo,errFlag);
  m_minuit->mnparm(2, "t0", vstart[2], step[2], 0., 0.,errFlag);

  if(idiocy.do1D == 1) { 
    m_minuit->FixParameter(0);
    m_minuit->FixParameter(1);
    m_minuit->mnexcm("MINIMIZE", arglist, 0,errFlag);
    m_minuit->Release(0);
    m_minuit->Release(1);
  }

  m_minuit->SetErrorDef(0.5);

  m_minuit->mnexcm("MIGRAD", arglist, 0,errFlag);
  
  // Get the chisquare and errors
  double chisq;
  double edm;
  double errdef;
  int npari, nparx, icstat;
  m_minuit->mnstat(chisq, edm, errdef, npari, nparx, icstat);
 
  int fitretval = errFlag;
  idiocy.retval = fitretval;
  idiocy.chi2 = chisq;
  if (npari<0 || nparx < 0 || chisq < 0) std::cout  <<  " MdtSegmentT0Fitter MINUIT problem " << " chisq "<<chisq<<" npari "<<npari<<" nparx "<< nparx <<" fitretval "<< fitretval<< "\n";

  if (fitretval !=0 &&m_trace)  {
   std::cout  <<  " ALARM return value " << fitretval << "\n";
  }

  // Get the fit values
  double aret(0),aErr(0);
  m_minuit->GetParameter(0,aret,aErr); // theta returned
  double dtheta = aErr;
  
  double tana = tan(aret); // tangent of angle
  
  double ang = aret;  // between zero and pi
  cosin = cos(ang);
  sinus = sin(ang);
  if ( sinus < 0.0 ) {
    sinus = -sinus;
    cosin = -cosin;
  } else if ( sinus == 0.0 && cosin < 0.0 ) {
    cosin = -cosin;
  }
  ang = atan2(sinus, cosin);
  
  
  double b(0),bErr(0);
  m_minuit->GetParameter(1,b,bErr); // intercept

  
  double t0(0),t0Err(0);
  m_minuit->GetParameter(2,t0,t0Err);
  
  
  //    double y0 = b * cosin ; // parallel distance
  double dy0 = cosin * bErr - b * sinus * aErr;
  
  double del_t;
  if(idiocy.dointernal) del_t = fabs(t2r((t0+t0Err)) - t2r(t0));
  else del_t = fabs(recrad((t0+t0Err)) - recrad(t0));

  idiocy.t0shifterr = t0Err;

  d = b * cosin;
      
  double covar[3][3];
  m_minuit->mnemat(&covar[0][0],3);  // 3x3
  if(m_trace) {
    std::cout <<"COVAR  ";
    for(int it1=0; it1<3; it1++) {
      for(int it2=0; it2<3; it2++) {
        std::cout <<covar[it1][it2]<<" ";
      }
	 std::cout << "\n";
    }
  }
  
  if(m_trace) std::cout <<"after fit theta "<<ang<<" sinus "<<sinus<< " cosin "<< cosin << "\n"; 
  
  double chi2 = 0;
  unsigned int nhits(0);
  double yl;
  
  // calculate predicted hit positions from track parameters
  it = (*dchits).begin();
  it_end = (*dchits).end();
  if(m_trace) std::cout <<"------NEW HITS------\n";
  (*dchits_new).clear();
  
//  std::cout<<"t0 shift is "<<t0<<"\n";
  idiocy.t0shift = t0;
  
  for(int i=0; it!=it_end; ++it, ++i ){
    double rad, drad;
    
    if(idiocy.dointernal) rad = t2r(t[i]-t0);
    else rad = recrad(t[i]-t0);

    //    if(rad>16) {
    //      std::cout<<"fit rini "<<r[i]<<" tini " <<t[i]<<" r "<<rad<<" t "<<t[i]-t0 << " t0 "<<t0<<" chisq "<<chisq<<" npari "<<npari<<" nparx "<< nparx <<" fitretval "<< fitretval<< "\n";
    //    }
    
    drad = 1.0/sqrt(w[i]) ; 

    yl = (y[i] -  tana*z[i] - b);
    if(m_trace) {
      std::cout <<"i "<<i<<" "; // ": yi "<<y[i]<<" line y "<<tana*z[i] + b<<" perp "<<yl/sqrt((1+tana*tana))<<" ";
    }

    double dth = -(sinus*y[i] + cosin*z[i])*dtheta;
    double residuals = fabs(yl)/sqrt((1+tana*tana)) - rad;
    if(m_trace) {
      std::cout <<" dth "<<dth<<" dy0 "<<dy0<<" del_t "<<del_t<<" ";
    }
    
    double errorResiduals = sqrt( dth*dth + dy0*dy0 + del_t*del_t);
    
    // derivatives of the residual 'R'
    double deriv[3];
    // del R/del theta
    double dd = z[i] * sinus + b *cosin - y[i] * cosin;
    deriv[0] = sign(dd) * (z[i] * cosin - b * sinus + y[i] * sinus);
    // del R / del b
    deriv[1] = sign(dd) * cosin ;
    // del R / del t0
    
    if(idiocy.dointernal) deriv[2] = -1* t2rprime(t[i]-t0);
    else deriv[2] = -1* recres(t[i]-t0);

    
    double covsq=0;
    for(int rr=0; rr<3; rr++) {
      for(int cc=0; cc<3; cc++) {
        covsq += deriv[rr]*covar[rr][cc]* deriv[cc];
      }
    }
    if(m_trace) std::cout << " covsquared " << covsq << "\n";
    covsq = sqrt(covsq);
    
    DCOnTrack tmp;
    tmp.x = (*dchits)[i].x;
    tmp.y = (*dchits)[i].y;
    tmp.t = (*dchits)[i].t-t0;
    tmp.r = rad;
    tmp.dr = drad;
        
    if(m_trace) std::cout  <<"T0 Segment hit res "<<residuals<<" eres "<<errorResiduals<<" covsq "<<covsq<<" ri " << r[i]<<" ro "<<rad<<" drad "<<drad<<" inv error " << w[i] << "\n";
    
    ++nhits;
    chi2 += residuals*residuals*w[i];
    (*dchits_new).push_back( tmp );
  }
  
  // Final Minuit Fit result
  xf = Zc - sinus*d;
  yf = Yc + cosin*d;
  thetaf = ang;

  idiocy.d = d;
  idiocy.Zc = Zc;
  idiocy.Yc = Yc;
  
  delete[] data;
  return true;
}


//////////////////////////////////////////// DISPLAY //////////////////////////////////////////////
