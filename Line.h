#ifndef DCMATH_LINE_H
#define DCMATH_LINE_H 1

#include <cmath>
#include <ostream>
#include <iostream>

#include <ostream>
#include <cmath>

class RotPhi {
public:
  RotPhi( double phi )
    : m_phi(phi),m_cosphi( cos(phi) ),m_sinphi( sin(phi) ) {}
  
  double phi() const { return m_phi; }
  double cosphi() const { return m_cosphi; }
  double sinphi() const { return m_sinphi; }
  
private:
  double m_phi;
  double m_cosphi;
  double m_sinphi;
};



  class LocVec2D{
  public:
    LocVec2D( double x, double y ) 
      : m_x(x),m_y(y) {}
  
    ~LocVec2D() {}

    double x() const { return m_x; }
    double y() const { return m_y; }

    LocVec2D operator+( const LocVec2D& lv ) const {
      return LocVec2D( x()+lv.x(), y()+lv.y() ); }

    LocVec2D operator-( const LocVec2D& lv ) const {
      return LocVec2D( x()-lv.x(), y()-lv.y() ); }

    LocVec2D& operator+=( const LocVec2D& lv ) {
      m_x+=lv.x(); m_y+=lv.y();
      return *this; }

    LocVec2D& operator-=( const LocVec2D& lv ) {
      m_x-=lv.x(); m_y-=lv.y();
      return *this; }

    double operator*( const LocVec2D& lv ) const {
      return x()*lv.x() + y()*lv.y(); }
  
    LocVec2D operator*( double v ) const {
      LocVec2D n(*this); return n*=v; }

    LocVec2D& operator*=( double v ) {
      m_x*=v; m_y*=v; return *this; }

    LocVec2D& operator/=( double v ) {
      m_x/=v; m_y/=v; return *this; }

  private:
    double m_x;
    double m_y;
  };

  typedef LocVec2D LocPos;
  typedef LocVec2D LocDir;


  class Line{
  public:
    Line( LocPos pos, double phi) : m_phi(phi),m_pos(pos) {  
      RotPhi rot( m_phi );
      LocPos p = LocVec2D( rot.cosphi()*pos.x() + rot.sinphi()*pos.y(), -rot.sinphi()*pos.x() + rot.cosphi()*pos.y() ) ;
      m_x0 = p.x();
      m_y0 = p.y();
    }

    ~Line() {}

    double phi() const { return m_phi; }
    double x0() const { return m_x0; }
    double y0() const { return m_y0; }

  private:
    double m_phi;
    LocPos m_pos;
    double m_x0;
    double m_y0;
  };

#endif

