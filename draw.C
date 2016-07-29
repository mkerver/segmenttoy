#include "draw.h"
#include "segment.h"
#include "chambergeometry.h"

#include "TEllipse.h"
#include "TLine.h"
#include "TArrow.h"
#include "TCanvas.h"
#include <math.h>
#include <iostream>
#include "defines.h"

//#define RAD 15
//#define SPACE 170
#define EPS 15

TEllipse *tubes[80][8];
TArrow *xaxis, *yaxis;



// width and height of canvas
int wid;
int hei;

TCanvas* makecanvas(char* name, int w, int h, bool showit) {
  wid = w;
  hei = h;

  if(showit) {
    TCanvas *cc = new TCanvas(name, name, 0, 0, 1400, floor(1400.0*(h+2*EPS)/(w+2*EPS)));  
    cc->SetFixedAspectRatio();  
    cc->Range(-EPS,-EPS,w+EPS,h+EPS);
    xaxis = new TArrow(40*RAD, 2.7*RAD + SPACE/2, 45*RAD, 2.7*RAD + SPACE/2, 0.05, ">");
    yaxis = new TArrow(40*RAD, 2.7*RAD + SPACE/2, 40*RAD, 7.7*RAD + SPACE/2, 0.05, ">");
    return cc;
  }
  else return NULL; 
}

TEllipse* maketube(int x, int y) {
  double xc, yc;
  getxy(xc, yc, x, y); 
  TEllipse *t = new TEllipse(xc,yc,RAD,RAD);  
  t->SetLineWidth(1);
  return t;
}

void makechamber() {
  for(int i=0; i<TUBES_PER_LAYER; i++) {
    for(int j=0; j<LAYERS; j++) {
      tubes[i][j]=maketube(i+1,j+1);
    }
  }
}

void drawchamber(bool newevent) {
  if(newevent) tubes[1][1]->Draw();
  for(int i=0; i<TUBES_PER_LAYER; i++) {
    for(int j=0; j<LAYERS; j++) {
      tubes[i][j]->SetLineColor(kBlack);
      tubes[i][j]->Draw("same");
    }
  }
  //  xaxis->Draw("same");
  //  yaxis->Draw("same");
}

void getmidpoint(segment &seg, double &xm, double &ym) {
  double seg_x[2]; //={-1, -1};
  double seg_y[2]; //={-1, -1};
  getlinepoints(&seg, seg_x, seg_y);
  xm = (seg_x[0] + seg_x[1])/2.0;
  ym = (seg_y[0] + seg_y[1])/2.0;
}

// places where the line crosses the canvas boundary
int getlinepoints(segment *seg, double *p_x, double *p_y) {
  double d;
  p_x[0]=-1; 
  p_y[0]=-1;
  p_x[1]=-1;
  p_y[1]=-1;
  int count=0;
  
  d = seg->getxat(0); // y=0  coordinate
  if(d>=0 && d<=wid) {
    p_x[count] = d;
    p_y[count] = 0;
    count++;
  }
  d = seg->getxat(hei); // y=height coordinate
  if(d>=0 && d<=wid) {
    p_x[count] = d;
    p_y[count] = hei;
    count++;
  }
  d = seg->getyat(0); // x=0  coordinate
  if(d>=0 && d<=hei) {
    p_x[count] = 0;
    p_y[count] = d;
    count++;
  }
  d = seg->getyat(wid); // x=width  coordinate
  if(d>=0 && d<=hei) {
    p_x[count] = wid;
    p_y[count] = d;
    count++;
  }
  
  if(count != 2) {
    std::cout<<"WTF? count is "<<count<<"\n";
std::cout<<seg->getyat(0)<<"  "<<hei<<"\n";
    return 0;
  }
  return 1;
}

TLine* drawsegment(segment *seg, bool before) {
  double seg_x[2]; //={-1, -1};
  double seg_y[2]; //={-1, -1};
  getlinepoints(seg, seg_x, seg_y);
  TLine *li = new TLine(seg_x[0], seg_y[0], seg_x[1], seg_y[1]);
  if(before) {
    li->SetLineWidth(3);
    if(before) li->SetLineColor(kRed);
  } else {
    li->SetLineWidth(2);
    li->SetLineColor(kBlue);
  }
  li->Draw("same");
  return li;
}

