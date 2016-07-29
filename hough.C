#include "hough.h"
#include "TMath.h"
#include "TH2D.h"
#include "TCanvas.h"
#include <iostream>

const int t_bins=25;
const int p_bins=150;
const double t_min = 0; 
const double t_max = 3.14159;
const double p_max = 2000;

double h_theta[t_bins];
double h_costheta[t_bins];
double h_sintheta[t_bins];
TH2D *h_hough; 

void h_clear() {
  for(int i=1; i <= t_bins; i++) {
    for(int j=1; j <= p_bins; j++) {
      h_hough->SetBinContent(i, j, 0);
    }
  }
}

void h_init() {
  for(int i=0; i<t_bins; i++) {
    double th = i*TMath::Pi()*1.0/t_bins;
    h_theta[i] = th;
    h_costheta[i] = TMath::Cos(th);
    h_sintheta[i] = TMath::Sin(th);
  }
  h_hough = new TH2D("h_hough", "h_hough", t_bins, t_min, t_max, p_bins, 0, p_max) ;
  //  for(int i=0; i<t_bins; i++ ) {
  //    std::cout<<i<<" "<<h_theta[i]<<" cos "<<h_costheta[i]<<" sin "<<h_sintheta[i]<<"\n";
  //  }
  h_hough->SetMarkerSize(1);
  h_hough->SetMarkerStyle(20);

}

void h_max(int &bi, int &bj) {
  double maxval=0;
  bi=-1, bj=-1;
  for(int i=1; i <= t_bins; i++) {
    for(int j=1; j <= p_bins; j++) {
      if(h_hough->GetBinContent(i, j) >= maxval) {
	maxval = h_hough->GetBinContent(i, j);
	bi = i;
	bj = j;
      }
    }
  }
}

double h_angle(int bin) {
  return (bin + 0.5) * (t_max - t_min) / t_bins; 
}

double h_p(int bin) {
  return (bin + 0.5) * p_max / p_bins; 
}


TH2D* h_pattern(const DCOnTrackVec& vec) {
  h_clear();
  int num = vec.size();
  double dx, dy, slope, ang;
  for(int i=0; i<num; i++) {
    for(int th=0; th<t_bins; th++) {
      //      for(int pb=0; pb<p_bins; pb++) {
      double p = vec[i].x * h_costheta[th] + vec[i].y * h_sintheta[th] ;
      h_hough->Fill(h_theta[th], p);
	//      }
    }
  }
  int bi, bj;
  h_max(bi, bj);
  std::cout<<" histo integral "<<h_hough->Integral();
  std::cout<<" bi  "<<bi<<" bj "<<bj;
  std::cout<<" max  "<<h_hough->GetBinContent(bi, bj);
  std::cout<<" theta "<<h_angle(bi) + TMath::Pi()/2;
  std::cout<<" perp "<<h_p(bi)<<"\n";
  return h_hough;
}
