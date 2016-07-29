#include "TLine.h"
#include "TEllipse.h"
#include "TCanvas.h"
#include "TColor.h"
#include "TApplication.h"
#include "TLegend.h"

#include <fstream>

#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctime>

#include <vector>

#include "michiganrt.h"
#include "dc.h"
#include "fit.h"
#include "rt.h"

#include "readdata.h"


#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"

#include "random.h"
#include "segment.h"
#include "chambergeometry.h"
#include "draw.h"
//#include "pattern.h"
#include "hough.h"
#include "defines.h"

#include "TNtuple.h"

double TDCBINSIZE=0.78125;

#define NUMPAR 3

typedef std::vector<DCOnTrack> DCOnTrackVec;

#define INNER_RAD 14.7

int width = ceil(XMAX); 
int height = ceil(YMAX);

bool drawit=false;

TCanvas *cc;
TCanvas *c1;
TCanvas *c2;
TCanvas *h_canv;
TLine *li;
TLine *li_new;
TFile *f;
TH1D* gaus_25, *gaus_78;

int numhits;

DCOnTrackVec dchits;
DCOnTrackVec dchits_new;

int doamt, dojitter, doangle, domezz, dointernal, printit, doposition, domidline, jitterrange;
bool doreaddata;
char simrt[80], recrt[80];

double intersect(segment *seg, int i, int j) {
  double xc, yc; 
  getxy(xc, yc, i, j);
  return seg->distancetopoint(xc, yc) ;
}
using namespace std;
double mezzoffsets[16] = {-7.69, -5.51, -5.16, -5.45, 0.09, -1.64, 0.27, 0.90, -0.06, 3.04, 4.08, 5.46, 2.57, 3.91, 2.35, 2.85};

void decideradiustime(double truer, double &r, double &t, double jitter, int mezz) {
  if(dointernal) t = r2t(truer) -jitter;
  else t = simtime(truer) -jitter;

  if(domezz) t = t - mezzoffsets[mezz];
  if(doamt) t = floor(t/TDCBINSIZE + 0.5) * TDCBINSIZE ;
  
  if(dointernal) r = t2r(t);
  else  r = simrad(t );
}

int permezz[16];
int nummezz;
// take a segment, and make a list of hits it crosses
void passhits(segment *seg, double jitter) {
  dchits.clear();
  numhits=0;
  for(int i=0; i<=15; i++) permezz[i]=0;
  DCOnTrack tmp;
  for(int i=0; i<TUBES_PER_LAYER; i++) {
    for(int j=0; j<LAYERS; j++) {
      int mezz = getmezz(i+1, j+1);
      double dist = intersect(seg, i+1, j+1);
      if(dist<=INNER_RAD) {
	permezz[mezz]++;
        double a, b;
        getxy(a, b, i+1, j+1);
        tmp.x = a;
        tmp.y = b;
        tmp.r = dist;
        tmp.rtrue = dist;
	if(dointernal) tmp.ttrue = r2t(dist);
        else tmp.ttrue = simtime(dist);
        tmp.dr = 2.;
        decideradiustime(dist, tmp.r, tmp.t, jitter, mezz) ;
        if(tmp.r>16) {
          std::cout<<"passhits rini "<<dist<<" t " <<tmp.t<<" r "<<tmp.r<<"\n";
        }
        dchits.push_back(tmp);
        numhits++;
      }
    }
  }
  nummezz=0;
  for(int i=0; i<=15; i++) if(permezz[i]>0) nummezz++;
}

// draw hits in list
std::vector<TEllipse* > ellipsevec;
void drawhits(DCOnTrackVec *hits, Color_t col) {
  for(int i=0; i<numhits; i++) {    
    TEllipse *hi = new TEllipse((*hits)[i].x, (*hits)[i].y, (*hits)[i].r, (*hits)[i].r);
    hi->SetLineWidth(1);
    hi->SetLineColor(col);
    hi->Draw("same");
    ellipsevec.push_back(hi);
  }
}

void deletehits() {
  for(int i=0; i<ellipsevec.size(); i++) {
    TEllipse *h = ellipsevec[i];
    delete h;
  }
  delete li;
  delete li_new;
  ellipsevec.clear();
}

void printhits() {
  if(dchits.size() != dchits_new.size() ) std::cout<<"Hit collections have different sizes!!\n";
  for(int i=0; i<numhits; i++ ) {
    if(dchits[i].x != dchits_new[i].x) std::cout<<"wtf? x differs\n";
    if(dchits[i].y != dchits_new[i].y) std::cout<<"wtf? y differs\n";    
    std::cout<<i<<": old is "<<dchits[i].r<<" new is "<<dchits_new[i].r<<" difference "<<dchits[i].r - dchits_new[i].r<<"\n";
  }
}

void parseopt(int disp) {
  fstream inp("options.txt", std::ios::in);
  inp>>dojitter;
  inp>>doamt;
  inp>>idiocy.do1D;
  inp>>doangle;
  inp>>doposition;
  inp>>domezz;
  inp>>dointernal;
  inp>>domidline;
  inp>>doreaddata;
  inp>>drawit;
  inp>>printit;
  inp>>jitterrange;
  inp>>simrt;
  inp>>recrt;
  inp.close();
  if(disp) {
    std::cout<<"Add Jitter "<<dojitter<<"\n";
    std::cout<<"Discretize AMT time "<<doamt<<"\n";
    std::cout<<"Do 1D prefit "<<idiocy.do1D<<"\n";
    std::cout<<"Smear seed angle "<<doangle<<"\n";
    std::cout<<"Smear seed position "<<doposition<<"\n";
    std::cout<<"Make mezz corrections "<<domezz<<"\n";
    std::cout<<"Use internal RT "<<dointernal<<"\n";
    std::cout<<"Mid-chamber segment generation "<<domidline<<"\n";
    std::cout<<"Read data "<<doreaddata<<"\n";
    std::cout<<"Display segments "<<drawit<<"\n";
    std::cout<<"Print segments "<<printit<<"\n";
    std::cout<<"Simulation RT function "<<simrt<<"\n";
    std::cout<<"Reconstruction RT function "<<recrt<<"\n";
  }
  readin(simrt, 0);
  readin(recrt, 1);
  idiocy.dointernal = dointernal;
}

int main(int argc, char** argv) {  
  TApplication theApp("App", &argc, argv);
  parseopt(1);
  
  srand(1);
  cc = makecanvas("test", width, height, drawit || printit);
  //  std::cout<<"Canvas made\n";
  makechamber();
  //  std::cout<<"Chamber made\n";
		
//   TFile *freso = new TFile("resos.root");
//   gaus_25 = (TH1D*) freso->Get("gaus_25");
//   gaus_78 = (TH1D*) freso->Get("gaus_78");


  readdata cosdata("seghits.txt");
  
  int flag1count=0, flag2count=0, flag3count=0;
  
  segment segt, segi, sego;
  
  TFile *f = new TFile("segments.root","RECREATE");
  TCanvas *c1 = new TCanvas("c1","c1");
  TCanvas *c2 = new TCanvas("c2","c2");
  TLegend *leg = new TLegend(0.1,0.8,0.3,1.0);
  TLegend *leg2 = new TLegend(0.1,0.8,0.3,1.0);
  float tmp[32];
  float tmpseg[20];

  int failcount=0;

  //  h_init();
   
  //TTree *t1 = new TTree("t1", "t1");
  TH1F *AMTresi = new TH1F("AMTresi","AMTresi",100,-1,1); 
  TH1F *positionresi = new TH1F("positionresi","positionresi",60,-3,3); 
  TH1F *Mezzresi = new TH1F("Mezzresi","Mezzresi",60,-3,3); 
  TH1F *jitterresi50 = new TH1F("jitterresi50","Residual",60,-3,3); 
  TH1F *jitterresi = new TH1F("jitterresi","Jitterresi",60,-3,3); 
  TH1F *AMTresi1= new TH1F("AMTresi1","AMTresi",100,-1,1); 
  TH1F *AMTresi2= new TH1F("AMTresi2","AMTresi",100,-1,1); 
  TH1F *AMTresi3= new TH1F("AMTresi3","AMTresi",100,-1,1); 

  TNtuple *nt = new TNtuple("nt","nt","thetat:thetai:thetao:nh:ri:ro:ti:to:jitter:dr:sdr:rt:tt:chi2:shift:retval:fcncount:angle:rest:resi:reso:nummezz:xt:xi:xo:yt:yi:yo:d:Zc:Yc:posdeform");
  TNtuple *se = new TNtuple("se","se","thetat:thetai:thetao:xt:xi:xo:yt:yi:yo:d:Zc:Yc:nh:jitter:shift:chi2:retval:fcncount:nummezz:dshift");
 
  double fitline_x, fitline_y, fitline_theta;
  double jitter;
  double xmid, ymid; // midpoint of segment
  double xdisp, ydisp; // displacement of midpoint position
  double angle; // rotation about midpoint position
  bool flag1, flag2, flag3;    
  double avg, var;
  double xtmid, ytmid; // midpoint of true segment
  double xomid, yomid; // midpoint of final segment

  int cou = 0;
  bool isdone = false;
  bool istheremoredata=true;


//   double testx, testy;
//   std::cout<<"In main\n";
//   getxy(testx, testy, 44, 1);
//   std::cout<<"testx "<<testx<<" testy "<<testy<<"\n";
//   getxy(testx, testy, 45, 1);
//   std::cout<<"testx "<<testx<<" testy "<<testy<<"\n";
//   getxy(testx, testy, 45, 2);
//   std::cout<<"testx "<<testx<<" testy "<<testy<<"\n";
//   getxy(testx, testy, 47, 3);
//   std::cout<<"testx "<<testx<<" testy "<<testy<<"\n";
//   getxy(testx, testy, 48, 3);
//   std::cout<<"testx "<<testx<<" testy "<<testy<<"\n";
//   getxy(testx, testy, 49, 3);
//   std::cout<<"testx "<<testx<<" testy "<<testy<<"\n";
//   getxy(testx, testy, 50, 3);
//   std::cout<<"testx "<<testx<<" testy "<<testy<<"\n";
 for(int nopt=0;nopt<8;nopt++){
//  for(int nopt=1;nopt<2;nopt++){
  isdone=false;
  dojitter=0;
  doamt=0;
  doposition=0;
  domezz=0;
  if(nopt==0)dojitter=1;
  if(nopt==1)doamt=1;
  if(nopt==2)doposition=1;
  if(nopt==3) domezz=1;
  if(nopt==4) dojitter=1;

  if(nopt==5){
      TDCBINSIZE=12.5;
      doamt=1;
    }
  if(nopt==6){
      TDCBINSIZE=25;
      doamt=1;
    }
  if(nopt==7){
      TDCBINSIZE=6.25;
      doamt=1;
    }
 while(!isdone) {
    if(!doreaddata) {
      if(dojitter && nopt==4) jitter = randDouble()*2.0*jitterrange - jitterrange;
      else if(dojitter) jitter = randDouble()*100.0-50.0;
      else jitter = 0;
      if(!domidline) segt.makerandomsegment(XMAX, YMAX);
      else segt.makemidlinesegment(XMAX, YMAX);
      getmidpoint(segt, xmid, ymid);   
      if(doposition) {
	xdisp = 4*randDouble()*RAD - 2*RAD;
	ydisp = 4*randDouble()*RAD - 2*RAD;
      } else {
	xdisp = 0;
	ydisp = 0;
      }
      if(doangle) {
      angle = randDouble()*0.174532925 - 0.0872664626;
      } else angle = 0;
      segi.makesegment(xmid + xdisp, ymid + ydisp, segt.theta() + angle);
      passhits(&segt, jitter);
    } else {
      istheremoredata = cosdata.nextsegment(segi, dchits);
      if(!istheremoredata) break;
      segt.makesegment(segi.x(), segi.y(), segi.theta());
      numhits = cosdata.getnh();
    }
/*
drawit=false;
for(int j=0;j<numhits; j++) {
if(segi.residual(dchits[j].x, dchits[j].y, dchits[j].r)<-0.05){
drawit=1;
cout<<segi.residual(dchits[j].x, dchits[j].y, dchits[j].r)<<" numhit:"<<j<<endl;
}
}*/
    if(drawit || printit) {
      cc->Clear();
      cc->Update();
    }
    if(drawit) {
      drawchamber(1);
      li = drawsegment(&segi, true);
      if(!doreaddata) std::cout<<"Numhits "<<numhits<<" true angle "<<segt.theta()<<" seed angle "<<segi.theta()<<" jitter "<<jitter<<" ";
      else std::cout<<"Numhits "<<numhits<<" data angle "<<segi.theta()<<" data shift "<<cosdata.getshift()<<" ";
      drawhits(&dchits, kRed);
      cc->Draw();
      cc->Update();
      std::cout<<"First draw\n";
      getchar();
    }  
  
    bool ret = fit(segi.x(), segi.y(), segi.theta(), &dchits, fitline_x, fitline_y, fitline_theta, &dchits_new);
    sego.makesegment(fitline_x, fitline_y, fitline_theta);

    if(drawit) std::cout<<"t0 shift is "<<idiocy.t0shift<<" fit angle "<<sego.theta()<<"\n";
    
    if(idiocy.retval!=0 && numhits>=5) {
      failcount++;
      //      if(drawit) std::cout<<"fit failed\n";
    }

    avg=0;
    var=0;
    flag1=false;
    flag2=false;
    flag3=false;
    for(int j=0; ret && j<numhits; j++) {
      double dr = dchits_new[j].r - dchits[j].r;
      avg = (avg*j + dr) / (j + 1);
      if (j > 0) var += (var * (j - 1) + (dr - avg)*(dr - avg)) / j;
      dr = abs(dr);
      if(dr<0.005) flag1=true;
      if(dr>0.005 && dr < 0.2) flag2 = true;
      if(dr>0.2) flag3=true; 
    }
    var = sqrt(var);

    getmidpoint(segt, xtmid, ytmid);   
    getmidpoint(sego, xomid, yomid);
    {
      int seloop=0;
      tmpseg[seloop++] = segt.theta();
      tmpseg[seloop++] = segi.theta();
      tmpseg[seloop++] = sego.theta();
      tmpseg[seloop++] = xtmid;
      tmpseg[seloop++] = segi.x(); 
      tmpseg[seloop++] = xomid;
      tmpseg[seloop++] = ytmid;
      tmpseg[seloop++] = segi.y(); 
      tmpseg[seloop++] = yomid;
      tmpseg[seloop++] = idiocy.d;
      tmpseg[seloop++] = idiocy.Zc;
      tmpseg[seloop++] = idiocy.Yc;
      tmpseg[seloop++] = numhits;
      tmpseg[seloop++] = jitter;
      tmpseg[seloop++] = idiocy.t0shift;
      tmpseg[seloop++] = idiocy.chi2;
      tmpseg[seloop++] = idiocy.retval;
      tmpseg[seloop++] = idiocy.fcncount;
      tmpseg[seloop++] = nummezz;
      tmpseg[seloop++] = idiocy.t0shifterr;
      se->Fill(tmpseg);
    }

    for(int j=0; ret && j<numhits; j++) {
      int ntloop=0;
      tmp[ntloop++] = segt.theta();
      tmp[ntloop++] = segi.theta();
      tmp[ntloop++] = sego.theta();
      tmp[ntloop++] = numhits;
      tmp[ntloop++] = dchits[j].r;
      tmp[ntloop++] = dchits_new[j].r;
      tmp[ntloop++] = dchits[j].t;
      tmp[ntloop++] = dchits_new[j].t;
      tmp[ntloop++] = jitter;
      tmp[ntloop++] = avg;
      tmp[ntloop++] = var;
      tmp[ntloop++] = dchits[j].rtrue;
      tmp[ntloop++] = dchits[j].ttrue;
      tmp[ntloop++] = idiocy.chi2;
      tmp[ntloop++] = idiocy.t0shift;
      tmp[ntloop++] = idiocy.retval;
      tmp[ntloop++] = idiocy.fcncount;
      tmp[ntloop++] = angle;
      tmp[ntloop++] = segt.residual(dchits[j].x, dchits[j].y, dchits[j].rtrue);
      tmp[ntloop++] = segi.residual(dchits[j].x, dchits[j].y, dchits[j].r);
      tmp[ntloop++] = sego.residual(dchits_new[j].x, dchits_new[j].y, dchits_new[j].r);
      tmp[ntloop++] = nummezz;
      tmp[ntloop++] = xtmid; // segt.x();
      tmp[ntloop++] = segi.x(); 
      tmp[ntloop++] = xomid; // sego.x();
      tmp[ntloop++] = ytmid; // segt.y();
      tmp[ntloop++] = segi.y(); 
      tmp[ntloop++] = yomid; //  segt.y();
      tmp[ntloop++] = idiocy.d;
      tmp[ntloop++] = idiocy.Zc;
      tmp[ntloop++] = idiocy.Yc;
      tmp[ntloop++] = sqrt((xtmid-segi.x())*(xtmid-segi.x())+(xtmid-segi.x())*(xtmid-segi.x()));

if(nopt==0)jitterresi50->Fill(segi.residual(dchits[j].x, dchits[j].y, dchits[j].r));
if(nopt==1)AMTresi->Fill(segi.residual(dchits[j].x, dchits[j].y, dchits[j].r));
//if(nopt==2)positionresi->Fill(segi.residual(dchits[j].x, dchits[j].y, dchits[j].r));
if(nopt==3)Mezzresi->Fill(segi.residual(dchits[j].x, dchits[j].y, dchits[j].r));
if(nopt==4)jitterresi->Fill(segi.residual(dchits[j].x, dchits[j].y, dchits[j].r));
if(nopt==5)AMTresi1->Fill(segi.residual(dchits[j].x, dchits[j].y, dchits[j].r));
if(nopt==6)AMTresi2->Fill(segi.residual(dchits[j].x, dchits[j].y, dchits[j].r));
if(nopt==7)AMTresi3->Fill(segi.residual(dchits[j].x, dchits[j].y, dchits[j].r));

      if(ret) nt->Fill(tmp);      
    } // hits ntuple filled 
    if(flag1) flag1count++;
    if(flag2) flag2count++;
    if(flag3) flag3count++;
    
    if(ret && drawit) {
      li->Draw("same");
      li_new = drawsegment(&sego, false) ;
      drawhits(&dchits_new, kBlue);
    }
    
    if(drawit || printit) {
      cc->Draw();
      cc->Update();
      if(printit) cc->Print(Form("tes/disp_%d.png", cou));
      if(printit) cc->Print(Form("tes/disp_%d.C", cou));
      std::cout<<"New seg, before delete\n";

      if(drawit) {
        cc->Update();
        getchar();
      }
      deletehits();
      if(drawit) getchar();
    }
 
    cou++;
    if((cou+1)%40000 == 0) std::cout<<"##\n";   
    else if((cou+1)%10000 == 0) std::cout<<"++ "<<std::flush;
    else if((cou+1)%2000 == 0) std::cout<<"* "<<std::flush;
    else if((cou+1)%400 == 0) std::cout<<"."<<std::flush;
    if(cou == 40000){
         isdone = true;
         cou=0;
      }
  } // segment loop done
c1->cd();
if(nopt==0){
jitterresi50->SetLineColor(kRed);
jitterresi50->SetLineWidth(1);
jitterresi50->Draw("same");
}
if(nopt==1){
AMTresi->SetLineColor(kGreen);
AMTresi->SetLineWidth(1);
AMTresi->Draw("same");
}
if(nopt==2){
positionresi->SetLineColor(kBlue);
positionresi->SetLineWidth(1);
positionresi->Draw("same");
}
if(nopt==3){
Mezzresi->SetLineColor(6);
Mezzresi->SetLineWidth(1);
Mezzresi->Draw("same");
}
if(nopt==4){
jitterresi->SetLineColor(7);
jitterresi->SetLineWidth(1);
jitterresi->Draw("same");
}
//c1->Update();
c2->cd();
if(nopt==1){
AMTresi->SetLineColor(kBlue);
AMTresi->SetLineWidth(1);
AMTresi->Draw("same");
}
if(nopt==5){
AMTresi1->SetLineColor(kGreen);
AMTresi1->SetLineWidth(1);
AMTresi1->Draw("same");
}
if(nopt==6){
AMTresi2->SetLineColor(kRed);
AMTresi2->SetLineWidth(1);
AMTresi2->Draw("same");
}
if(nopt==7){
AMTresi3->SetLineColor(7);
AMTresi3->SetLineWidth(1);
AMTresi3->Draw("same");
}
cout<<"option "<<nopt<<" complete"<<endl;
}

  leg2->AddEntry(AMTresi,"0.78125","l");
  leg2->AddEntry(AMTresi3,"6.25","l");
  leg2->AddEntry(AMTresi1,"12.5","l");
  leg2->AddEntry(AMTresi2,"25","l");
  leg2->Draw();
  c2->SetLogy();
  c1->cd();
  leg->AddEntry(jitterresi50,"Jitter100","l");
  leg->AddEntry(AMTresi,"AMT Discretize","l");
  leg->AddEntry(positionresi,"Position smear","l");
  leg->AddEntry(Mezzresi,"Mezz correction","l");
  leg->AddEntry(jitterresi,"Jitter","l");
  leg->Draw();
  c1->SetLogy();
  f->Write();
  std::cout<<failcount<<" fits failed\n";
  c1->Write();
  c2->Write();
  return 0;
}
