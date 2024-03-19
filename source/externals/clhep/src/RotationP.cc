#include "CommonHeader.h"

// -*- C++ -*-
// ---------------------------------------------------------------------------
//
// This file is a part of the CLHEP - a Class Library for High Energy Physics.
//
// This is the implementation of methods of the HepRotation class which
// were introduced when ZOOM PhysicsVectors was merged in, other than those
// involving Euler or axis/angle representations, lengthy corrections of
// the rotation matrix, or I/O.
//

#include "CLHEP/Vector/Rotation.h"

#include <cmath>


namespace CLHEP  {

void HepRotation::decompose(HepAxisAngle & rotation, Hep3Vector & boost)const {
  boost.set(0,0,0);
  rotation = axisAngle();
}

void HepRotation::decompose(Hep3Vector & boost, HepAxisAngle & rotation)const {
  boost.set(0,0,0);
  rotation = axisAngle();
}

G4double HepRotation::distance2( const HepRotation & r  ) const {
  G4double sum = rxx * r.rxx + rxy * r.rxy + rxz * r.rxz
                + ryx * r.ryx + ryy * r.ryy + ryz * r.ryz
                + rzx * r.rzx + rzy * r.rzy + rzz * r.rzz;
  G4double answer = 3.0 - sum;
  return (answer >= 0 ) ? answer : 0;
}

G4double HepRotation::howNear(   const HepRotation & r  ) const {
  return  std::sqrt( distance2( r ) );
}

bool HepRotation::isNear(   const HepRotation & r,
                                     G4double epsilon) const {
 return  distance2( r ) <= epsilon*epsilon;
}

G4double HepRotation::norm2() const {
  G4double answer = 3.0 - rxx - ryy - rzz;
  return (answer >= 0 ) ? answer : 0;
}

}  // namespace CLHEP
