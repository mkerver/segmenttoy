{
  TNtuple *nt = (TNtuple*) gDirectory->Get("nt");
  nt->Draw("reso>>h1", "retval==0 && nummezz==1 && abs(reso)<0.01");
  nt->Draw("reso>>h2", "retval==0 && nummezz==2 && abs(reso)<0.1");
  nt->Draw("reso>>h2a", "retval==0 && nummezz==2 && abs(reso)<0.1 && abs(thetai-1.5708)<0.3491");
  nt->Draw("reso>>h2b", "retval==0 && nummezz==2 && abs(reso)<0.1 && abs(thetai-1.5708)>1.2217");
  nt->Draw("reso>>h3", "retval==0 && nummezz==3 && abs(reso)<0.2");
  nt->Draw("reso>>h4", "retval==0 && nummezz==4 && abs(reso)<0.4");
  nt->Draw("reso>>h5", "retval==0 && nummezz==5 && abs(reso)<0.5");
  nt->Draw("reso>>h6", "retval==0 && nummezz==6 && abs(reso)<0.5");
  nt->Draw("reso>>h7", "retval==0 && nummezz==7 && abs(reso)<0.5");
  nt->Draw("reso>>h8", "retval==0 && nummezz==8 && abs(reso)<0.5");

  h1 = (TH1*) gDirectory->Get("h1");
  h2 = (TH1*) gDirectory->Get("h2");
  h2a = (TH1*) gDirectory->Get("h2a");
  h2b = (TH1*) gDirectory->Get("h2b");
  h3 = (TH1*) gDirectory->Get("h3");
  h4 = (TH1*) gDirectory->Get("h4");
  h5 = (TH1*) gDirectory->Get("h5");
  h6 = (TH1*) gDirectory->Get("h6");
  h7 = (TH1*) gDirectory->Get("h7");
  h8 = (TH1*) gDirectory->Get("h8");

  h1->GetXaxis()->SetTitle("residual [mm]");
  h2->GetXaxis()->SetTitle("residual [mm]");
  h2a->GetXaxis()->SetTitle("residual [mm]");  
  h2b->GetXaxis()->SetTitle("residual [mm]");  
  h3->GetXaxis()->SetTitle("residual [mm]");
  h4->GetXaxis()->SetTitle("residual [mm]");
  h5->GetXaxis()->SetTitle("residual [mm]");  
  h6->GetXaxis()->SetTitle("residual [mm]");
  h7->GetXaxis()->SetTitle("residual [mm]");
  h8->GetXaxis()->SetTitle("residual [mm]");  

  h1->SetTitle("residual, # mezz = 1");
  h2->SetTitle("residual, # mezz = 2");
  h2a->SetTitle("residual, # mezz = 2, within 20 deg of vertical");  
  h2b->SetTitle("residual, #mezz = 2, within 20 deg of horizontal");  
  h3->SetTitle("residual, # mezz = 3");
  h4->SetTitle("residual, # mezz = 4");
  h5->SetTitle("residual, # mezz = 5");  
  h6->SetTitle("residual, # mezz = 6");
  h7->SetTitle("residual, # mezz = 7");
  h8->SetTitle("residual, # mezz = 8");  

  h1->SetLineColor(kPumpkin);
  h2->SetLineColor(kPumpkin);
  h2a->SetLineColor(kPumpkin);
  h2b->SetLineColor(kPumpkin);
  h3->SetLineColor(kPumpkin);
  h4->SetLineColor(kPumpkin);
  h5->SetLineColor(kPumpkin);
  h6->SetLineColor(kPumpkin);
  h7->SetLineColor(kPumpkin);
  h8->SetLineColor(kPumpkin);

  h1->SetLineWidth(2);
  h2->SetLineWidth(2);
  h2a->SetLineWidth(2);
  h2b->SetLineWidth(2);
  h3->SetLineWidth(2);
  h4->SetLineWidth(2);
  h5->SetLineWidth(2);
  h6->SetLineWidth(2);
  h7->SetLineWidth(2);
  h8->SetLineWidth(2);

  /*
  h1->DrawNormalized();
  h2->DrawNormalized("same");
  h3->DrawNormalized("same");
  h4->DrawNormalized("same");
  h5->DrawNormalized("same");
  h6->DrawNormalized("same");
  h7->DrawNormalized("same");
  h8->DrawNormalized("same");
  */

  gStyle->SetOptStat(111);
  gStyle->SetOptFit(111);

  
  h1->Fit("gaus");
  h1->GetFunction("gaus")->SetLineWidth(2);
  h1->GetFunction("gaus")->SetLineColor(kEmerald);

  h2->Fit("gaus", "R", "", -0.02, 0.02);
  h2->GetFunction("gaus")->SetLineWidth(2);
  h2->GetFunction("gaus")->SetLineColor(kEmerald);

  //  h2a->Fit("gaus", "R", "", -0.02, 0.02);
  //  h2a->GetFunction("gaus")->SetLineWidth(2);
  //  h2a->GetFunction("gaus")->SetLineColor(kEmerald);

  //  h2b->Fit("gaus");
  //  h2b->GetFunction("gaus")->SetLineWidth(2);
  //  h2b->GetFunction("gaus")->SetLineColor(kEmerald);

  h3->Fit("gaus", "R", "", -0.05, 0.05);
  h3->GetFunction("gaus")->SetLineWidth(2);
  h3->GetFunction("gaus")->SetLineColor(kEmerald);

  h4->Fit("gaus", "R", "", -0.1, 0.1);
  h4->GetFunction("gaus")->SetLineWidth(2);
  h4->GetFunction("gaus")->SetLineColor(kEmerald);

  h5->Fit("gaus", "R", "", -0.15, 0.15);
  h5->GetFunction("gaus")->SetLineWidth(2);
  h5->GetFunction("gaus")->SetLineColor(kEmerald);

  h6->Fit("gaus", "R", "", -0.2, 0.2);
  h6->GetFunction("gaus")->SetLineWidth(2);
  h6->GetFunction("gaus")->SetLineColor(kEmerald);

  h7->Fit("gaus", "R", "", -0.2, 0.2);
  h7->GetFunction("gaus")->SetLineWidth(2);
  h7->GetFunction("gaus")->SetLineColor(kEmerald);

  h8->Fit("gaus", "R", "", -0.25, 0.25);
  h8->GetFunction("gaus")->SetLineWidth(2);
  h8->GetFunction("gaus")->SetLineColor(kEmerald);

  
}
