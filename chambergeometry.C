#include "chambergeometry.h"
#include <iostream>
#include <math.h>
#include "defines.h"


// given tube and layer numbers, return the x, y coordinate of the wire
// tube numbers start at
// layer numbers start at 
void getxy(double &x, double &y, int i, int j) {
  int ml = (j>LAYERS_PER_ML?2:1);
  x = (2*i -1 + (j-1)%2) * RAD;
  y = (1 + (j-1)*SQRT3) * RAD + (ml-1)*SPACE;
}

int getmezz(int i, int j) {
  int ml = (j>LAYERS_PER_ML?1:0);
  int bin = (int) ceil((i-1)/8);
  return 2*bin + ml;
}

void showmezzlayers() {
  for (int j=1; j<=LAYERS; j++) {
    for (int i=1; i<=TUBES_PER_LAYER; i++) {
      std::cout<<getmezz(i, j)<<" ";
    }
    std::cout<<"\n";
  }
}
