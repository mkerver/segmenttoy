#ifndef HOUGH_H
#define HOUGH_H 1
#include "dc.h"
#include "TH2D.h"

void h_init();

TH2D* h_pattern(const DCOnTrackVec& vec);

#endif
