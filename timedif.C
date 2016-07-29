#include "TFile.h"
#include "TNtuple.h"
#include "TH1D.h"
#include <iostream>
#include <TCanvas.h>
#include <fstream>
#include "TSlider.h"

#include "TBox.h"
#include "TPad.h"
#include<cstdlib>


TFile *f[337];

TNtuple *nt[337];
TH1D *htm[337];
TBox *box;
TPad *pa;

TCanvas *c1;

void timedif() {
  c1 = new TCanvas("c1", "c1", 600, 600);
  //  pa = new TPad("pa", "pa", 0.85, 0.1, 0.9, 0.9);


  fstream fil("7day/rms.out", std::ios::out);

  //  TSlider *slider= new TSlider("slider","test",4.2,0,4.6, 0.9);
  box= new TBox(0.01,0.01,0.99, 0.99);
  box->SetFillColor(kRed);
  
  //slider->SetFillColor(46);
  //  gSystem->Unlink("7daymovie.gif");


  for(int i=0; i<337; i++) {
//     pa->cd();
//     box->Draw();
//     c1->cd(0);
    f[i] = new TFile(Form("7day/segments%d.root", i));
    nt[i] = (TNtuple*) f[i]->Get("nt");
    htm[i] = new TH1D(Form("htm%d", i), Form("htm%d", i), 100, -0.25, 0.25);
    nt[i]->Draw(Form("reso>>htm%d", i), "retval==0 && abs(reso)<0.25");
    htm[i]->GetXaxis()->SetTitle("residual [mm]");
    htm[i]->SetTitle(Form("Residual, one week difference, iteration %d", i));
    //    pa->Draw("same");

    c1->Modified();
    c1->Update();
    c1->Paint();
    c1->Print("7daymovie.gif+");
    //    std::cout<<"print\n"<<std::flush;

        c1->Print(Form("7day/res%d.C", i)); 
        c1->Print(Form("7day/res%d.eps", i));
        c1->Print(Form("7day/res%d.png", i));
        fil<<i<<" mean "<<htm[i]->GetMean()<<" RMS "<<htm[i]->GetRMS()<<"\n";

    f[i]->Close("R");
    //    nt[i]->Delete();
  }
  //  slider->SetRange(0,1);
  // total->Draw("sameaxis"); // to redraw axis hidden by the fill area
  c1->Modified();
  // make infinite animation by adding "++" to the file name
  c1->Print("7daymovie.gif+");

}
