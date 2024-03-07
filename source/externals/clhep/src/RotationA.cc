// -*- C++ -*-
// ---------------------------------------------------------------------------
//
// This file is a part of the CLHEP - a Class Library for High Energy Physics.
//
// This is the implementation of those methods of the HepRotation class which
// were introduced when ZOOM PhysicsVectors was merged in, and which involve 
// the angle/axis representation of a Rotation.
//

#include "CLHEP/Vector/Rotation.h"
#include "CLHEP/Units/PhysicalConstants.h"

#include <iostream>
#include <cmath>

namespace CLHEP  {

// ----------  Constructors and Assignment:

// axis and angle

HepRotation & HepRotation::set( const Hep3Vector & aaxis, G4double ddelta ) {

  G4double sinDelta = std::sin(ddelta), cosDelta = std::cos(ddelta);
  G4double oneMinusCosDelta = 1.0 - cosDelta;

  Hep3Vector u = aaxis.unit();

  G4double uX = u.getX();
  G4double uY = u.getY();
  G4double uZ = u.getZ();

  rxx = oneMinusCosDelta * uX * uX  +  cosDelta;
  rxy = oneMinusCosDelta * uX * uY  -  sinDelta * uZ;
  rxz = oneMinusCosDelta * uX * uZ  +  sinDelta * uY;

  ryx = oneMinusCosDelta * uY * uX  +  sinDelta * uZ;
  ryy = oneMinusCosDelta * uY * uY  +  cosDelta;
  ryz = oneMinusCosDelta * uY * uZ  -  sinDelta * uX;

  rzx = oneMinusCosDelta * uZ * uX  -  sinDelta * uY;
  rzy = oneMinusCosDelta * uZ * uY  +  sinDelta * uX;
  rzz = oneMinusCosDelta * uZ * uZ  +  cosDelta;

  return  *this;

} // HepRotation::set(axis, delta)

HepRotation::HepRotation ( const Hep3Vector & aaxis, G4double ddelta ) 
{
  set( aaxis, ddelta );
}  
HepRotation & HepRotation::set( const HepAxisAngle & ax ) {
  return  set ( ax.axis(), ax.delta() );
}
HepRotation::HepRotation ( const HepAxisAngle & ax ) 
{
  set ( ax.axis(), ax.delta() );
}

G4double    HepRotation::delta() const {

  G4double cosdelta = (rxx + ryy + rzz - 1.0) / 2.0;
  if (cosdelta > 1.0) {
    return 0;
  } else if (cosdelta < -1.0) {
    return CLHEP::pi;
  } else {
    return  std::acos( cosdelta ); // Already safe due to the cosdelta > 1 check
  }

} // delta()

Hep3Vector HepRotation::axis () const {

  const G4double eps = 1e-15;

  G4double Ux = rzy - ryz;
  G4double Uy = rxz - rzx;
  G4double Uz = ryx - rxy;
  if (std::abs(Ux) < eps && std::abs(Uy) < eps && std::abs(Uz) < eps) {

    G4double cosdelta = (rxx + ryy + rzz - 1.0) / 2.0;
    if (cosdelta > 0.0) return Hep3Vector(0,0,1); // angle = 0, any axis is good

    G4double mxx = (rxx + 1)/2;
    G4double myy = (ryy + 1)/2;
    G4double mzz = (rzz + 1)/2;
    G4double mxy = (rxy + ryx)/4;
    G4double mxz = (rxz + rzx)/4;
    G4double myz = (ryz + rzy)/4;
    G4double x, y, z;

    if (mxx > ryy && mxx > rzz) {
      x = std::sqrt(mxx);
      if (rzy - ryz < 0) x = -x;
      y = mxy/x;
      z = mxz/x;
      return  Hep3Vector( x, y, z ).unit();
    } else if (myy > mzz) {
      y = std::sqrt(myy);
      if (rxz - rzx < 0) y = -y;
      x = mxy/y;
      z = myz/y;
      return  Hep3Vector( x, y, z ).unit();
    } else {
      z = std::sqrt(mzz);
      if (ryx - rxy < 0) z = -z;
      x = mxz/z;
      y = myz/z;
      return  Hep3Vector( x, y, z ).unit();
    }
  } else {
    return  Hep3Vector( Ux, Uy, Uz ).unit();
  }

} // axis()

HepAxisAngle HepRotation::axisAngle() const {

  return HepAxisAngle (axis(), delta());

} // axisAngle() 


void HepRotation::setAxis (const Hep3Vector & aaxis) {
  set ( aaxis, delta() );
}

void HepRotation::setDelta (G4double ddelta) {
  set ( axis(), ddelta );
}

}  // namespace CLHEP
