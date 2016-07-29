{
  TFile f("segments.root");
  TNtuple *nt = (TNtuple*)f.Get("nt");
  gInterpreter->LoadMacro("michiganrt.C");
  gInterpreter->LoadMacro("rt.C");

  readin("Feb20/RT_run91060_EMS1A12.dat", 0) ;
  readin("Feb20/RT_run91060_EMS5A04.dat", 1) ;
  
  TF1 *f_cr = new TF1("f_cr", TF1t2r, 0, 800, 0) ;
  TF1 *f_ja = new TF1("f_ja", TF1simrad, 0, 800, 0) ;
  TF1 *f_au = new TF1("f_au", TF1recrad, 0, 800, 0) ;

  f_cr->SetLineColor(kBlack);
  f_ja->SetLineColor(kRed);
  f_au->SetLineColor(kBlue);
  f_cr->SetLineWidth(2);
  f_ja->SetLineWidth(2);
  f_au->SetLineWidth(2);

  f_cr->Draw();
  f_ja->Draw("same");
  //  f_au->Draw("same");

  c1->Print("rtfunc.png");
  c1->Print("rtfunc.eps");
  c1->Print("rtfunc.C");

//   TH1D *h_rtrue = new TH1D("h_rtrue", "Radius", 100, 0, 15.5);
//   TH1D *h_rinit = new TH1D("h_rinit", "Radius", 100, 0, 15.5); 
//   TH1D *h_rfina = new TH1D("h_rfina", "Radius", 100, 0, 15.5);

//   nt->Draw("rt>>h_rtrue", "");
//   nt->Draw("ri>>h_rinit", "");
//   nt->Draw("ro>>h_rfina", "");
  
//   h_rtrue->SetLineColor(kBlack);
//   h_rinit->SetLineColor(kRed);
//   h_rfina->SetLineColor(kBlue);
//   h_rtrue->SetLineWidth(2);
//   h_rinit->SetLineWidth(2);
//   h_rfina->SetLineWidth(2);

//   h_rfina->Draw();
//   h_rinit->Draw("same");
//   h_rtrue->Draw("same");

//   c1->Print("rad.png");
}

