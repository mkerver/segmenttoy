#ifndef RT_H
#define RT_H 1

#define MAX_DRIFT 855

// garbage time value to return when radius isn't wihin rt range
#define R2TSPURIOUS 50000

double t2r(double tin) ;
double t2rprime(double tin) ;
double r2t(double r) ;

#endif
