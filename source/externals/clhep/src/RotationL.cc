#include "CommonHeader.h"

// -*- C++ -*-
// ---------------------------------------------------------------------------
//
// This file is a part of the CLHEP - a Class Library for High Energy Physics.
//
// This is the implementation of methods of the HepRotation class which
// were introduced when ZOOM PhysicsVectors was merged in, which might cause 
// pulling in of LorentzTransformation related code units.
//

#include "CLHEP/Vector/Rotation.h"
#include "CLHEP/Vector/LorentzRotation.h"

#include <cmath>

namespace CLHEP  {

// ----------  distance2 and related member functions:
//
// WHy do we have forms for HepLorentzRotation and HepBoost but not for 
// HepBoostX, HepBoostY, HepBoostZ?  Because the latter can be gotten by 
// implicit conversion to HepBoost; but if we just had HepLorentzRotation 
// then this would involve G4double conversion when HepBoostX was used.

G4double HepRotation::distance2( const HepLorentzRotation & lt  ) const {
  HepAxisAngle a; 
  Hep3Vector   b;
  lt.decompose(b, a);
  G4double bet = b.beta();
  G4double bet2 = bet*bet;
  HepRotation r(a);
  return bet2/(1-bet2) + distance2(r);
}

G4double HepRotation::distance2( const HepBoost & lt ) const {
  return distance2( HepLorentzRotation(lt));
}

G4double HepRotation::howNear( const HepLorentzRotation & lt  ) const {
  return  std::sqrt( distance2( lt ) );
}

G4double HepRotation::howNear( const HepBoost & lt  ) const {
  return  std::sqrt( distance2( lt ) );
}

bool HepRotation::isNear(   const HepLorentzRotation & lt,
                                     G4double epsilon) const {
 return  distance2( lt ) <= epsilon*epsilon;
}

bool HepRotation::isNear(   const HepBoost & lt,
                                     G4double epsilon) const {
 return  distance2( lt ) <= epsilon*epsilon;
}

}  // namespace CLHEP

