#include "TH1D.h"
#include "TF1.h"
#include "TNtuple.h"
#include "TFile.h"
#include <iostream>
#include "/Users/srivasprasad/.root/macros/func.C"

#define N 20000

TH1D *hgau[N];
TF1 *fshape, *ffit;
TH1D *hmeanpul, *hsigpul;
TH1D *hmean, *hmeanerr;
TH1D *hsig, *hsigerr;

TFile *fil;
TNtuple *gp;


void gauspul() {
  fil = new TFile("gauspul.root", "RECREATE");
  gp = new TNtuple("gp","gp","mt:st:mf:sf:mfe:sfe");
  fshape=new TF1("fshape", gauss, -100, 100, 3);
  // shape function
  fshape->SetParameter(0, 10);
  fshape->SetParameter(1, 0.5); // mean. between -5 and 5
  fshape->SetParameter(2, 1.5); // sigma. between -5 and 5

  hmeanpul = new TH1D("hmeanpul", "hmeanpul", 100, -5, 5);
  hsigpul = new TH1D("hsigpul", "hsigpul", 100, -5, 5);  

  hmean = new TH1D("hmean", "hmean", 1000, -5, 5);
  hmeanerr = new TH1D("hmeanerr", "hmeanerr", 1000, -5, 5);
  hsig = new TH1D("hsig", "hsig", 1000, -5, 5);
  hsigerr = new TH1D("hsigerr", "hsigerr", 1000, -5, 5);


  TF1 *ffit = new TF1("ffit", gauss, -100, 100, 3);

  for(int i=0; i<N; i++) {
    hgau[i] = new TH1D(Form("hgau%d", i), Form("hgau%d", i), 1000, -100, 100);
    hgau[i]->FillRandom("fshape", 20000);
    ffit->SetParameter(0, 800);
    ffit->SetParameter(1, hgau[i]->GetMean());
    ffit->SetParameter(2, hgau[i]->GetRMS());

    if((i+1)%2000==0) std::cout<<".\n";
    else if((i+1)%1000==0) std::cout<<"*"<<std::flush;
    else if((i+1)%100==0) std::cout<<"."<<std::flush;


    hgau[i]->Fit("ffit", "LQ0");
    hmean->Fill(ffit->GetParameter(1));
    hmeanerr->Fill(ffit->GetParError(1));
    hsig->Fill(ffit->GetParameter(2));
    hsigerr->Fill(ffit->GetParError(2));
    
    hmeanpul->Fill((ffit->GetParameter(1)-fshape->GetParameter(1))/ffit->GetParError(1));
    hsigpul ->Fill((ffit->GetParameter(2)-fshape->GetParameter(2))/ffit->GetParError(2));
    gp->Fill(fshape->GetParameter(1), fshape->GetParameter(2), ffit->GetParameter(1), ffit->GetParameter(2), ffit->GetParError(1), ffit->GetParError(2));
  }
  for(int i=0; i<N; i++) hgau[i]->Delete();

  fil->Write();

}
