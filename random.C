#include "random.h"

#include <cstdlib>
//#include <stdio.h>
//#include <stdlib.h>
//#include <math.h>

double randDouble() {
  return ( static_cast<double>(rand()) / (static_cast<double>(RAND_MAX)+static_cast<double>(1)) );
}
