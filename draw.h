#ifndef DRAW_H
#define DRAW_H

class TCanvas ;
class segment ;
class TLine ;

TCanvas* makecanvas(char* name, int w, int h, bool showit) ; 
void makechamber() ;
void drawchamber(bool) ;

int getlinepoints(segment *seg, double *p_x, double *p_y) ;
void getmidpoint(segment &seg, double &xm, double &ym) ;
TLine* drawsegment(segment *seg, bool before);

#endif
