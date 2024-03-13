// -*- C++ -*-
// ---------------------------------------------------------------------------

#include <cmath>
#include <float.h>
#include <iostream>
#include "CLHEP/Geometry/BasicVector3D.h"

namespace HepGeom {
  //--------------------------------------------------------------------------
  template<>
  G4float BasicVector3D<G4float>::pseudoRapidity() const {
    G4float ma = mag(), dz = z();
    if (ma ==  0)  return  0;
    if (ma ==  dz) return  FLT_MAX;
    if (ma == -dz) return -FLT_MAX;
    return 0.5*std::log((ma+dz)/(ma-dz));
  }

  //--------------------------------------------------------------------------
  template<>
  void BasicVector3D<G4float>::setEta(G4float a) {
    G4double ma = mag();
    if (ma == 0) return;
    G4double tanHalfTheta  = std::exp(-a);
    G4double tanHalfTheta2 = tanHalfTheta * tanHalfTheta;
    G4double cosTheta1      = (1 - tanHalfTheta2) / (1 + tanHalfTheta2);
    G4double rh            = ma * std::sqrt(1 - cosTheta1*cosTheta1);
    G4double ph            = phi();
    set(rh*std::cos(ph), rh*std::sin(ph), ma*cosTheta1);
  }

  //--------------------------------------------------------------------------
  template<>
  G4float BasicVector3D<G4float>::angle(const BasicVector3D<G4float> & v) const {
    G4double cosa = 0;
    G4double ptot = mag()*v.mag();
    if(ptot > 0) {
      cosa = dot(v)/ptot;
      if(cosa >  1) cosa =  1;
      if(cosa < -1) cosa = -1;
    }
    return std::acos(cosa);
  }

  //--------------------------------------------------------------------------
  template<>
  BasicVector3D<G4float> & BasicVector3D<G4float>::rotateX(G4float a) {
    G4double sina = std::sin(a), cosa = std::cos(a), dy = y(), dz = z();
    setY(dy*cosa-dz*sina);
    setZ(dz*cosa+dy*sina);
    return *this;
  }

  //--------------------------------------------------------------------------
  template<>
  BasicVector3D<G4float> & BasicVector3D<G4float>::rotateY(G4float a) {
    G4double sina = std::sin(a), cosa = std::cos(a), dz = z(), dx = x();
    setZ(dz*cosa-dx*sina);
    setX(dx*cosa+dz*sina);
    return *this;
  }

  //--------------------------------------------------------------------------
  template<>
  BasicVector3D<G4float> & BasicVector3D<G4float>::rotateZ(G4float a) {
    G4double sina = std::sin(a), cosa = std::cos(a), dx = x(), dy = y();
    setX(dx*cosa-dy*sina);
    setY(dy*cosa+dx*sina);
    return *this;
  }

  //--------------------------------------------------------------------------
  template<>
  BasicVector3D<G4float> &
  BasicVector3D<G4float>::rotate(G4float a, const BasicVector3D<G4float> & v) {
    if (a  == 0) return *this;
    G4double cx = v.x(), cy = v.y(), cz = v.z();
    G4double ll = std::sqrt(cx*cx + cy*cy + cz*cz);
    if (ll == 0) {
      std::cerr << "BasicVector<G4float>::rotate() : zero axis" << std::endl;
      return *this;
    }
    G4double cosa = std::cos(a), sina = std::sin(a);
    cx /= ll; cy /= ll; cz /= ll;   

    G4double xx = cosa + (1-cosa)*cx*cx;
    G4double xy =        (1-cosa)*cx*cy - sina*cz;
    G4double xz =        (1-cosa)*cx*cz + sina*cy;
    
    G4double yx =        (1-cosa)*cy*cx + sina*cz;
    G4double yy = cosa + (1-cosa)*cy*cy; 
    G4double yz =        (1-cosa)*cy*cz - sina*cx;
    
    G4double zx =        (1-cosa)*cz*cx - sina*cy;
    G4double zy =        (1-cosa)*cz*cy + sina*cx;
    G4double zz = cosa + (1-cosa)*cz*cz;

    cx = x(); cy = y(); cz = z();   
    set(xx*cx+xy*cy+xz*cz, yx*cx+yy*cy+yz*cz, zx*cx+zy*cy+zz*cz);
    return *this;
  }

  //--------------------------------------------------------------------------
  std::ostream &
  operator<<(std::ostream & os, const BasicVector3D<G4float> & a)
  {
    return os << "(" << a.x() << "," << a.y() << "," << a.z() << ")";
  }

  //--------------------------------------------------------------------------
  std::istream &
  operator>> (std::istream & is, BasicVector3D<G4float> & a)
  {
    // Required format is ( a, b, c ) that is, three numbers, preceded by
    // (, followed by ), and separated by commas.  The three numbers are
    // taken as x, y, z.

    G4float x, y, z;
    char c;

    is >> std::ws >> c;
    // ws is defined to invoke eatwhite(istream & )
    // see (Stroustrup gray book) page 333 and 345.
    if (is.fail() || c != '(' ) {
      std::cerr
	<< "Could not find required opening parenthesis "
	<< "in input of a BasicVector3D<G4float>"
	<< std::endl;
      return is;
    }

    is >> x >> std::ws >> c;
    if (is.fail() || c != ',' ) {
      std::cerr
	<< "Could not find x value and required trailing comma "
	<< "in input of a BasicVector3D<G4float>"
	<< std::endl; 
      return is;
    }

    is >> y >> std::ws >> c;
    if (is.fail() || c != ',' ) {
      std::cerr
	<< "Could not find y value and required trailing comma "
	<<  "in input of a BasicVector3D<G4float>"
	<< std::endl;
      return is;
    }

    is >> z >> std::ws >> c;
    if (is.fail() || c != ')' ) {
      std::cerr
	<< "Could not find z value and required close parenthesis "
	<< "in input of a BasicVector3D<G4float>"
	<< std::endl;
      return is;
    }

    a.setX(x);
    a.setY(y);
    a.setZ(z);
    return is;
  }
  
  //--------------------------------------------------------------------------
  template<>
  G4double BasicVector3D<G4double>::pseudoRapidity() const {
    G4double ma = mag(), dz = z();
    if (ma ==  0)  return  0;
    if (ma ==  dz) return  DBL_MAX;
    if (ma == -dz) return -DBL_MAX;
    return 0.5*std::log((ma+dz)/(ma-dz));
  }

  //--------------------------------------------------------------------------
  template<>
  void BasicVector3D<G4double>::setEta(G4double a) {
    G4double ma = mag();
    if (ma == 0) return;
    G4double tanHalfTheta  = std::exp(-a);
    G4double tanHalfTheta2 = tanHalfTheta * tanHalfTheta;
    G4double cosTheta1      = (1 - tanHalfTheta2) / (1 + tanHalfTheta2);
    G4double rh            = ma * std::sqrt(1 - cosTheta1*cosTheta1);
    G4double ph            = phi();
    set(rh*std::cos(ph), rh*std::sin(ph), ma*cosTheta1);
  }

  //--------------------------------------------------------------------------
  template<>
  G4double BasicVector3D<G4double>::angle(const BasicVector3D<G4double> & v) const {
    G4double cosa = 0;
    G4double ptot = mag()*v.mag();
    if(ptot > 0) {
      cosa = dot(v)/ptot;
      if(cosa >  1) cosa =  1;
      if(cosa < -1) cosa = -1;
    }
    return std::acos(cosa);
  }

  //--------------------------------------------------------------------------
  template<>
  BasicVector3D<G4double> & BasicVector3D<G4double>::rotateX(G4double a) {
    G4double sina = std::sin(a), cosa = std::cos(a), dy = y(), dz = z();
    setY(dy*cosa-dz*sina);
    setZ(dz*cosa+dy*sina);
    return *this;
  }

  //--------------------------------------------------------------------------
  template<>
  BasicVector3D<G4double> & BasicVector3D<G4double>::rotateY(G4double a) {
    G4double sina = std::sin(a), cosa = std::cos(a), dz = z(), dx = x();
    setZ(dz*cosa-dx*sina);
    setX(dx*cosa+dz*sina);
    return *this;
  }

  //--------------------------------------------------------------------------
  template<>
  BasicVector3D<G4double> & BasicVector3D<G4double>::rotateZ(G4double a) {
    G4double sina = std::sin(a), cosa = std::cos(a), dx = x(), dy = y();
    setX(dx*cosa-dy*sina);
    setY(dy*cosa+dx*sina);
    return *this;
  }

  //--------------------------------------------------------------------------
  template<>
  BasicVector3D<G4double> &
  BasicVector3D<G4double>::rotate(G4double a, const BasicVector3D<G4double> & v) {
    if (a  == 0) return *this;
    G4double cx = v.x(), cy = v.y(), cz = v.z();
    G4double ll = std::sqrt(cx*cx + cy*cy + cz*cz);
    if (ll == 0) {
      std::cerr << "BasicVector<G4double>::rotate() : zero axis" << std::endl;
      return *this;
    }
    G4double cosa = std::cos(a), sina = std::sin(a);
    cx /= ll; cy /= ll; cz /= ll;   

    G4double xx = cosa + (1-cosa)*cx*cx;
    G4double xy =        (1-cosa)*cx*cy - sina*cz;
    G4double xz =        (1-cosa)*cx*cz + sina*cy;
    
    G4double yx =        (1-cosa)*cy*cx + sina*cz;
    G4double yy = cosa + (1-cosa)*cy*cy; 
    G4double yz =        (1-cosa)*cy*cz - sina*cx;
    
    G4double zx =        (1-cosa)*cz*cx - sina*cy;
    G4double zy =        (1-cosa)*cz*cy + sina*cx;
    G4double zz = cosa + (1-cosa)*cz*cz;

    cx = x(); cy = y(); cz = z();   
    set(xx*cx+xy*cy+xz*cz, yx*cx+yy*cy+yz*cz, zx*cx+zy*cy+zz*cz);
    return *this;
  }

  //--------------------------------------------------------------------------
  std::ostream &
  operator<< (std::ostream & os, const BasicVector3D<G4double> & a)
  {
    return os << "(" << a.x() << "," << a.y() << "," << a.z() << ")";
  }
  
  //--------------------------------------------------------------------------
  std::istream &
  operator>> (std::istream & is, BasicVector3D<G4double> & a)
  {
    // Required format is ( a, b, c ) that is, three numbers, preceded by
    // (, followed by ), and separated by commas.  The three numbers are
    // taken as x, y, z.
    
    G4double x, y, z;
    char c;
    
    is >> std::ws >> c;
    // ws is defined to invoke eatwhite(istream & )
    // see (Stroustrup gray book) page 333 and 345.
    if (is.fail() || c != '(' ) {
      std::cerr
	<< "Could not find required opening parenthesis "
	<< "in input of a BasicVector3D<G4double>"
	<< std::endl;
      return is;
    }

    is >> x >> std::ws >> c;
    if (is.fail() || c != ',' ) {
      std::cerr
	<< "Could not find x value and required trailing comma "
	<< "in input of a BasicVector3D<G4double>"
	<< std::endl; 
      return is;
    }

    is >> y >> std::ws >> c;
    if (is.fail() || c != ',' ) {
      std::cerr
	<< "Could not find y value and required trailing comma "
	<<  "in input of a BasicVector3D<G4double>"
	<< std::endl;
      return is;
    }

    is >> z >> std::ws >> c;
    if (is.fail() || c != ')' ) {
      std::cerr
	<< "Could not find z value and required close parenthesis "
	<< "in input of a BasicVector3D<G4double>"
	<< std::endl;
      return is;
    }

    a.setX(x);
    a.setY(y);
    a.setZ(z);
    return is;
  }
} /* namespace HepGeom */
