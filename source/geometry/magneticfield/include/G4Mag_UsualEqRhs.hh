// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: G4Mag_UsualEqRhs.hh,v 1.3 2000/04/27 09:14:06 gcosmo Exp $
// GEANT4 tag $Name: geant4-03-01 $
//
//
// class G4Mag_UsualEqRhs
//
// Class description:
//
// This is the standard right-hand side for equation of motion.
// The only case another is required is when using a moving reference
// frame ... or extending the class to include additional Forces,
// eg an electric field

// History:
// - Created: J. Apostolakis, January 13th 1997.

#ifndef G4MAG_USUAL_EQRHS
#define G4MAG_USUAL_EQRHS

#include "G4Mag_EqRhs.hh"
#include "G4MagneticField.hh"

class G4Mag_UsualEqRhs : public G4Mag_EqRhs
{
   public:  // with description

     G4Mag_UsualEqRhs( G4MagneticField* MagField )
       : G4Mag_EqRhs( MagField ) {;}
    ~G4Mag_UsualEqRhs() {;}
       // Constructor and destructor. No actions.

     void EvaluateRhsGivenB( const G4double y[],
			     const G4double B[3],
				   G4double dydx[] ) const;
       // Given the value of the magnetic field B, this function 
       // calculates the value of the derivative dydx.
};

#endif /* G4MAG_USUAL_EQRHS */