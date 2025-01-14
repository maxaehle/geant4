#include "CommonHeader.h"

// -*- C++ -*-
// ---------------------------------------------------------------------------
//
// This file is a part of the CLHEP - a Class Library for High Energy Physics.
//
// This is the implementation of those few parts of the Hep4RotationInterface
// and Hep3RotationInterface classes which are neither inline nor pure virtual.
//

#include "CLHEP/Vector/RotationInterfaces.h"

namespace CLHEP  {

//-******************************
//
// Hep4RotationInterface
//
//-******************************

G4double Hep4RotationInterface::tolerance = 
			Hep4RotationInterface::ToleranceTicks * 1.0e-08;

G4double Hep4RotationInterface::getTolerance() {return tolerance;} 
G4double Hep4RotationInterface::setTolerance( G4double tol ) {
  G4double t = tolerance; tolerance = tol; return t;
}


//-******************************
//
// Hep3RotationInterface
//
//-******************************

}  // namespace CLHEP
