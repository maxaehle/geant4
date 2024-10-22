#include "CommonHeader.h"

//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// G4GeometryTolerance
//
// Class description:
//
// A singleton class for computation and storage of the tolerance values
// used by the geometry modeler for precision on boundaries.
// The Cartesian tolerance defines the thickness of the surface of the
// geometrical shapes for their inner funtions and for the tracking. The
// tolerance must be greater than the largest mathematical error from the
// shape distance calculation functions. The tolerance is centred on the
// surface, e.g. the Inside() method of a solid uses a tolerance dx +/- kTol/2.
// The Cartesian tolerance can either be set to a fixed value (1E-9 mm)
// or to a value computed on the basis of the maximum extent of the
// world volume assigned through the G4GeometryManager at the beginning
// of the application -before- any geometrical object is created.

//      ---------------- G4GeometryTolerance ----------------
//
// Author: G.Cosmo (CERN), 30 October 2006
// --------------------------------------------------------------------
#ifndef G4GeometryTolerance_hh
#define G4GeometryTolerance_hh

#include "G4Types.hh"

class G4GeometryTolerance
{
  friend class G4GeometryManager;

 public:  // with description
  static G4GeometryTolerance* GetInstance();
  // Get a pointer to the unique G4GeometryTolerance,
  // creating it if necessary and setting the tolerances.
  G4double GetSurfaceTolerance() const;
  // Returns the current Cartesian tolerance of a surface.
  G4double GetAngularTolerance() const;
  // Returns the current angular tolerance.
  G4double GetRadialTolerance() const;
  // Returns the current radial tolerance.

 public:  // without description
  ~G4GeometryTolerance();
  // Destructor.

 protected:
  void SetSurfaceTolerance(G4double worldExtent);
  // Sets the Cartesian and Radial surface tolerance to a value computed
  // from the maximum extent of the world volume. This method
  // can be called only once, and is done only through the
  // G4GeometryManager class.

  G4GeometryTolerance();
  // Protected constructor.

 private:
  static G4ThreadLocal G4GeometryTolerance* fpInstance;
  G4double fCarTolerance;
  G4double fAngTolerance;
  G4double fRadTolerance;
  G4bool fInitialised = false;
};

#endif
