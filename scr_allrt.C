{
  gInterpreter->LoadMacro("michiganrt.C");
  gInterpreter->LoadMacro("rt.C");

  const int NUM = 80;


  std::fstream readlist("bigwheel", std::ios::in);
  char name[80];

  TF1 *f_ja[NUM+1];
  TCanvas cc;
  cc.cd();
  cc.Clear();

  TF1 *f_cr = new TF1("f_cr", TF1t2r, 0, 800, 0) ; 
  f_cr->Draw();

  for(int i=0; i<NUM; i++) f_ja[i] = new TF1(Form("f_ja%d", i), TF1simrad, 0, 800, 0);

  int i=0;
  int min_i;
  int max_i;
  string min_name;
  string max_name;
  double min_y = 1e10;
  double max_y = -1e10;

  while(1) {
    readlist>>name;
    if(!readlist.good()) break;
    std::cout<<i<<" --"<<name<<"--"<<"\n";
    readin(Form("Feb20/%s", name), 0);
    f_ja[i]->SetLineWidth(2);
    f_ja[i]->SetLineColor(kBlue);
    double val = f_ja[i]->Eval(750);
    if(val >= max_y) {
      max_y = val;
      max_i = i;
      max_name = name;
    }
    if(val < min_y) {
      min_y = val;
      min_i = i;
      min_name = name;
    }
    i++;

  }
  cout<<"Max: "<<max_i<<" "<<max_name<<" "<<max_y<<"\n";
  cout<<"Min: "<<min_i<<" "<<min_name<<" "<<min_y<<"\n";

}

