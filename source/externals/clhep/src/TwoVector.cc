#include "CommonHeader.h"

// -*- C++ -*-
// ---------------------------------------------------------------------------
//
// This file is a part of the CLHEP - a Class Library for High Energy Physics.
//
// This is the implementation of the Hep2Vector class.
//
//-------------------------------------------------------------

#include "CLHEP/Vector/TwoVector.h"
#include "CLHEP/Vector/ThreeVector.h"

#include <cmath>
#include <iostream>

namespace CLHEP  {

G4double Hep2Vector::tolerance = Hep2Vector::ZMpvToleranceTicks * 2.22045e-16;

G4double Hep2Vector::setTolerance (G4double tol) {
// Set the tolerance for Hep2Vectors to be considered near one another
  G4double oldTolerance (tolerance);
  tolerance = tol;
  return oldTolerance;
}

G4double Hep2Vector::operator () (int i) const {
  if (i == 0) {
    return x();
  }else if (i == 1) {
    return y();
  }else{
//    std::cerr << "Hep2Vector::operator () - "
//              << "Hep2Vector::operator(): bad index" << std::endl;
    return 0.0;
  }
}

G4double & Hep2Vector::operator () (int i) {
  static G4double dummy;
  switch(i) {
  case X:
    return dx;
  case Y:
    return dy;
  default:
//    std::cerr << "Hep2Vector::operator () - "
//              << "Hep2Vector::operator() : bad index" << std::endl;
    return dummy;
  }
}

void Hep2Vector::rotate(G4double aangle) {
  G4double ss = std::sin(aangle);
  G4double cc = std::cos(aangle);
  G4double xx = dx;
  dx = cc*xx - ss*dy;
  dy = ss*xx + cc*dy;
}

Hep2Vector operator/ (const Hep2Vector & p, G4double a) {
//  if (a==0) {
//    std::cerr << "Hep2Vector operator/ () - "
//              << "Division of Hep2Vector by zero" << std::endl;
//  }
  return Hep2Vector(p.x()/a, p.y()/a);
}

std::ostream & operator << (std::ostream & os, const Hep2Vector & q) {
  os << "(" << q.x() << ", " << q.y() << ")";
  return os;
}

void ZMinput2G4doubles ( std::istream & is, const char * type,
                       G4double & x, G4double & y );

std::istream & operator>>(std::istream & is, Hep2Vector & p) {
  G4double x, y;
  ZMinput2G4doubles ( is, "Hep2Vector", x, y );
  p.set(x, y);
  return  is;
}  // operator>>()

Hep2Vector::operator Hep3Vector () const {
  return Hep3Vector ( dx, dy, 0.0 );
}

int Hep2Vector::compare (const Hep2Vector & v) const {
  if       ( dy > v.dy ) {
    return 1;
  } else if ( dy < v.dy ) {
    return -1;
  } else if ( dx > v.dx ) {
    return 1;
  } else if ( dx < v.dx ) {
    return -1;
  } else {
    return 0;
  }
} /* Compare */


bool Hep2Vector::operator > (const Hep2Vector & v) const {
	return (compare(v)  > 0);
}
bool Hep2Vector::operator < (const Hep2Vector & v) const {
	return (compare(v)  < 0);
}
bool Hep2Vector::operator>= (const Hep2Vector & v) const {
	return (compare(v) >= 0);
}
bool Hep2Vector::operator<= (const Hep2Vector & v) const {
	return (compare(v) <= 0);
}

bool Hep2Vector::isNear(const Hep2Vector & p, G4double epsilon) const {
  G4double limit = dot(p)*epsilon*epsilon;
  return ( (*this - p).mag2() <= limit );
} /* isNear() */

G4double Hep2Vector::howNear(const Hep2Vector & p ) const {
  G4double d   = (*this - p).mag2();
  G4double pdp = dot(p);
  if ( (pdp > 0) && (d < pdp)  ) {
    return std::sqrt (d/pdp);
  } else if ( (pdp == 0) && (d == 0) ) {
    return 0;
  } else {
    return 1;
  }
} /* howNear */

G4double Hep2Vector::howParallel (const Hep2Vector & v) const {
  // | V1 x V2 | / | V1 dot V2 |
  // Of course, the "cross product" is fictitious but the math is valid
  G4double v1v2 = std::fabs(dot(v));
  if ( v1v2 == 0 ) {
    // Zero is parallel to no other vector except for zero.
    return ( (mag2() == 0) && (v.mag2() == 0) ) ? 0 : 1;
  }
  G4double abscross = std::fabs ( dx * v.y() - dy - v.x() );
  if ( abscross >= v1v2 ) {
    return 1;
  } else {
    return abscross/v1v2;
  }
} /* howParallel() */

bool Hep2Vector::isParallel (const Hep2Vector & v,
			     G4double epsilon) const {
  // | V1 x V2 | <= epsilon * | V1 dot V2 | 
  // Of course, the "cross product" is fictitious but the math is valid
  G4double v1v2 = std::fabs(dot(v));
  if ( v1v2 == 0 ) {
    // Zero is parallel to no other vector except for zero.
    return ( (mag2() == 0) && (v.mag2() == 0) );
  }
  G4double abscross = std::fabs ( dx * v.y() - dy - v.x() );
  return ( abscross <= epsilon * v1v2 );
} /* isParallel() */

G4double Hep2Vector::howOrthogonal (const Hep2Vector & v) const {
  // | V1 dot V2 | / | V1 x V2 | 
  // Of course, the "cross product" is fictitious but the math is valid
  G4double v1v2 = std::fabs(dot(v));
  if ( v1v2 == 0 ) {
    return 0;	// Even if one or both are 0, they are considered orthogonal
  }
  G4double abscross = std::fabs ( dx * v.y() - dy - v.x() );
  if ( v1v2 >= abscross ) {
    return 1;
  } else {
    return v1v2/abscross;
  }
} /* howOrthogonal() */

bool Hep2Vector::isOrthogonal (const Hep2Vector & v,
			     G4double epsilon) const {
  // | V1 dot V2 | <= epsilon * | V1 x V2 | 
  // Of course, the "cross product" is fictitious but the math is valid
  G4double v1v2 = std::fabs(dot(v));
  G4double abscross = std::fabs ( dx * v.y() - dy - v.x() );
  return ( v1v2 <= epsilon * abscross );
} /* isOrthogonal() */

}  // namespace CLHEP
