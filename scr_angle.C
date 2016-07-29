{

TFile f00("segmentsnojitter_no1D.root");
TFile f01("segmentsnojitter_1D.root");

TFile f10("segments_no1D.root");
TFile f11("segments_1D.root");

TNtuple *nt00 = (TNtuple*) f00.Get("nt");
TNtuple *nt01 = (TNtuple*) f01.Get("nt");
TNtuple *nt10 = (TNtuple*) f10.Get("nt");
TNtuple *nt11 = (TNtuple*) f11.Get("nt");

nt00->SetLineColor(kBlue);
nt01->SetLineColor(kRed);
nt10->SetLineColor(kBlue);
nt11->SetLineColor(kRed);

nt00->SetLineWidth(2);
nt01->SetLineWidth(2);
nt10->SetLineWidth(2);
nt11->SetLineWidth(2);
}
