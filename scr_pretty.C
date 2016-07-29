{
  TFile f("segments.root");
  TNtuple *nt = (TNtuple*) f.Get("nt");
  gStyle->SetStatColor(kWhite);
  nt->Draw("reso>>all", "abs(reso)<10 && retval==0");
  all->SetLineColor(kPumpkin);
  all->SetLineWidth(2);
  all->GetXaxis()->SetTitle("residual [mm]");
  all->SetTitle("Fitted residual, AMT binning");
  all->SetStats(0);
  all->Draw();
  c1->SetLogy();
  c1->Print("amt_all.eps");
  c1->Print("amt_all.C");

  c1->SetLogy(0);
  nt->Draw("reso>>peak", "abs(reso)<0.05 && retval==0");
  peak->SetLineColor(kPumpkin);
  peak->SetLineWidth(2);
  peak->GetXaxis()->SetTitle("residual [mm]");
  peak->SetTitle("Residual, AMT binning");
  gStyle->SetOptStat(111);
  gStyle->SetOptFit(11);
  peak->Draw();
  peak->Fit("gaus");
  peak->GetFunction("gaus")->SetLineColor(kEmerald);
  peak->GetFunction("gaus")->SetLineWidth(2);
  peak->Draw();
  c1->Print("amt_fit.eps");
  c1->Print("amt_fit.C");

  cout<<"\n"<<1.0 * peak->Integral()/ all->Integral()<<"\n";
  /*
  nt->Draw("(thetat-thetao)>>angpeak", "abs(reso)<0.005 && retval==0 && abs(thetat-thetao)<0.000025");
  angpeak->SetLineColor(kPumpkin);
  angpeak->SetLineWidth(2);
  angpeak->GetXaxis()->SetTitle("#Delta #theta [radian]");
  angpeak->SetTitle("Smear angle");
  gStyle->SetOptStat(111);
  gStyle->SetOptFit(11);
  angpeak->Draw();
  angpeak->Fit("gaus", "R", "", -0.000003, 0.000003);
  angpeak->GetFunction("gaus")->SetLineColor(kEmerald);
  angpeak->GetFunction("gaus")->SetLineWidth(2);
  angpeak->Draw();
  c1->Print("angle_theta_fit.png");
  c1->Print("angle_theta_fit.C");
  */
}
