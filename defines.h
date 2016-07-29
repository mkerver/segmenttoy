#ifndef DEFINES_H
#define DEFINES_H 1

#define SQRT3 1.7320508076
#define RAD 14.985

/*
class chamberparams {
 public:
  const int LAYERS_PER_ML;
  const int TUBES_PER_LAYER;
  const double SPACE;
  const int LAYERS;
  chamberparams(int l, int t, double s, int nl) {
    LAYERS_PER_ML = l;
    TUBES_PER_LAYER = t;
    SPACE = s;
    LAYERS = nl;
  }
};
*/

//BIS
/*
const int LAYERS_PER_ML = 4;
const int TUBES_PER_LAYER = 30;
const double SPACE = 6.5;
const int LAYERS = (2*LAYERS_PER_ML);
//*/

//BIL
/*
const int LAYERS_PER_ML = 4;
const int TUBES_PER_LAYER = 36;
const double SPACE = 170;
const int LAYERS = (2*LAYERS_PER_ML);
//*/

//BMS
/*
const int LAYERS_PER_ML = 3;
const int TUBES_PER_LAYER = 48;
const double SPACE = 170;
const int LAYERS = (2*LAYERS_PER_ML);
//*/

//BML
/*
const int LAYERS_PER_ML = 3;
const int TUBES_PER_LAYER = 56;
const double SPACE = 317;
const int LAYERS = (2*LAYERS_PER_ML);
//*/

//BO
/*
const int LAYERS_PER_ML = 3;
const int TUBES_PER_LAYER = 72;
const double SPACE = 317;
const int LAYERS = (2*LAYERS_PER_ML);
//*/

//EIS 
/*
const int LAYERS_PER_ML = 4;
const int TUBES_PER_LAYER = 36;
const int SPACE = 121;
const int LAYERS = (2*LAYERS_PER_ML);
//*/

//EIL 
//*
const int LAYERS_PER_ML = 4;
const int TUBES_PER_LAYER = 54;
const int SPACE = 121;
const int LAYERS = (2*LAYERS_PER_ML);
//*/

// EMS EML
/*
const int LAYERS_PER_ML = 3;
const int TUBES_PER_LAYER = 64;
const int SPACE = 170;
const int LAYERS = (2*LAYERS_PER_ML);
//*/

// EOS EOL
/*
const int LAYERS_PER_ML = 3;
const int TUBES_PER_LAYER = 48;
const int SPACE = 170;
const int LAYERS = (2*LAYERS_PER_ML);
//*/

const int ML_THICKNESS = (2*RAD + RAD*SQRT3*(LAYERS_PER_ML - 1)) ;
const int XMAX = ((2*RAD*TUBES_PER_LAYER)+RAD) ;
const int YMAX = (2*ML_THICKNESS + SPACE) ;

#endif
