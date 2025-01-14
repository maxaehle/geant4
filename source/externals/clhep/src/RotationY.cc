#include "CommonHeader.h"

// -*- C++ -*-
// ---------------------------------------------------------------------------
//
// This file is a part of the CLHEP - a Class Library for High Energy Physics.
//
// This is the implementation of methods of the HepRotationY class which
// were introduced when ZOOM PhysicsVectors was merged in.
//

#include "CLHEP/Vector/RotationY.h"
#include "CLHEP/Vector/AxisAngle.h"
#include "CLHEP/Vector/EulerAngles.h"
#include "CLHEP/Vector/LorentzRotation.h"
#include "CLHEP/Units/PhysicalConstants.h"

#include <cmath>
#include <stdlib.h>
#include <iostream>

namespace CLHEP  {

static inline G4double safe_acos (G4double x) {
  if (std::abs(x) <= 1.0) return std::acos(x);
  return ( (x>0) ? 0 : CLHEP::pi );
}

HepRotationY::HepRotationY(G4double ddelta) : 
		its_d(proper(ddelta)), its_s(std::sin(ddelta)), its_c(std::cos(ddelta))
{}

HepRotationY & HepRotationY::set ( G4double ddelta ) {
  its_d = proper(ddelta);
  its_s = std::sin(its_d);
  its_c = std::cos(its_d);
  return *this;
}

G4double  HepRotationY::phi() const {
  if ( its_d == 0 ) {
    return 0;
  } else if ( (its_d < 0) || (its_d == CLHEP::pi) )  {
    return +CLHEP::halfpi;
  } else {
    return -CLHEP::halfpi;
  }
}  // HepRotationY::phi()

G4double  HepRotationY::theta() const {
  return  std::fabs( its_d );
}  // HepRotationY::theta()

G4double  HepRotationY::psi() const {
  if ( its_d == 0 ) {
    return 0;
  } else if ( (its_d < 0) || (its_d == CLHEP::pi) )  {
    return -CLHEP::halfpi;
  } else {
    return +CLHEP::halfpi;
  }
}  // HepRotationY::psi()

HepEulerAngles HepRotationY::eulerAngles() const {
  return HepEulerAngles(  phi(),  theta(),  psi() );
}  // HepRotationY::eulerAngles()


// From the defining code in the implementation of CLHEP (in Rotation.cc)
// it is clear that thetaX, phiX form the polar angles in the original
// coordinate system of the new X axis (and similarly for phiY and phiZ).
//
// This code is taken directly from the original CLHEP. However, there are as
// shown opportunities for significant speed improvement.

G4double HepRotationY::phiX() const {
  return (yx() == 0.0 && xx() == 0.0) ? 0.0 : std::atan2(yx(),xx());
  		// or ---- return 0;
}

G4double HepRotationY::phiY() const {
  return (yy() == 0.0 && xy() == 0.0) ? 0.0 : std::atan2(yy(),xy());
		// or ----  return CLHEP::halfpi;
}

G4double HepRotationY::phiZ() const {
  return (yz() == 0.0 && xz() == 0.0) ? 0.0 : std::atan2(yz(),xz());
		// or ----  return 0;
}

G4double HepRotationY::thetaX() const {
  return safe_acos(zx());
}

G4double HepRotationY::thetaY() const {
  return safe_acos(zy());
		// or ----  return CLHEP::halfpi;
}

G4double HepRotationY::thetaZ() const {
  return safe_acos(zz());  
		// or ---- return d;
}

void HepRotationY::setDelta ( G4double ddelta ) {
  set(ddelta);
}

void HepRotationY::decompose
	(HepAxisAngle & rotation, Hep3Vector & boost) const {
  boost.set(0,0,0);
  rotation = axisAngle();
}

void HepRotationY::decompose
	(Hep3Vector & boost, HepAxisAngle & rotation) const {
  boost.set(0,0,0);
  rotation = axisAngle();
}

void HepRotationY::decompose
        (HepRotation & rotation, HepBoost & boost) const {
  boost.set(0,0,0);
  rotation = HepRotation(*this);
}
 
void HepRotationY::decompose
        (HepBoost & boost, HepRotation & rotation) const {
  boost.set(0,0,0);
  rotation = HepRotation(*this);
}

G4double HepRotationY::distance2( const HepRotationY & r  ) const {
  G4double answer = 2.0 * ( 1.0 - ( its_s * r.its_s + its_c * r.its_c ) ) ;
  return (answer >= 0) ? answer : 0;
}

G4double HepRotationY::distance2( const HepRotation & r  ) const {
  G4double sum =        xx() * r.xx()          +  xz() * r.xz()
		   		       + r.yy() 
                       + zx() * r.zx()          + zz() * r.zz();
  G4double answer = 3.0 - sum;
  return (answer >= 0 ) ? answer : 0;
}

G4double HepRotationY::distance2( const HepLorentzRotation & lt  ) const {
  HepAxisAngle a; 
  Hep3Vector   b;
  lt.decompose(b, a);
  G4double bet = b.beta();
  G4double bet2 = bet*bet;
  HepRotation r(a);
  return bet2/(1-bet2) + distance2(r);
}

G4double HepRotationY::distance2( const HepBoost & lt ) const {
  return distance2( HepLorentzRotation(lt));
}

G4double HepRotationY::howNear( const HepRotationY & r ) const {
  return std::sqrt(distance2(r));
}
G4double HepRotationY::howNear( const HepRotation & r ) const {
  return std::sqrt(distance2(r));
}
G4double HepRotationY::howNear( const HepBoost & lt ) const {
  return std::sqrt(distance2(lt));
}
G4double HepRotationY::howNear( const HepLorentzRotation & lt ) const {
  return std::sqrt(distance2(lt));
}
bool HepRotationY::isNear(const HepRotationY & r,G4double epsilon)const{
  return (distance2(r) <= epsilon*epsilon);
}
bool HepRotationY::isNear(const HepRotation & r,G4double epsilon)const {
  return (distance2(r) <= epsilon*epsilon);
}
bool HepRotationY::isNear( const HepBoost & lt,G4double epsilon) const {
  return (distance2(lt) <= epsilon*epsilon);
}
bool HepRotationY::isNear( const HepLorentzRotation & lt,
                                     G4double epsilon) const {
  return (distance2(lt) <= epsilon*epsilon);
}

G4double HepRotationY::norm2() const {
  return 2.0 - 2.0 * its_c;
}

std::ostream & HepRotationY::print( std::ostream & os ) const {
  os << "\nRotation about Y (" << its_d <<
                ") [cos d = " << its_c << " sin d = " << its_s << "]\n";
  return os;
}

}  // namespace CLHEP
