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
// G4tgrPlace
//
// Class description:
//
// Abstract base class to describe the position of a G4tgrVolume inside
// another G4tgrVolume.

// Author: P.Arce, CIEMAT (November 2007)
// --------------------------------------------------------------------
#ifndef G4tgrPlace_hh
#define G4tgrPlace_hh 1

#include "globals.hh"
#include "G4ThreeVector.hh"

class G4tgrVolume;

class G4tgrPlace
{
  public:

    G4tgrPlace();
    virtual ~G4tgrPlace();

    // Accessors

    const G4String& GetParentName() const { return theParentName; }
    G4tgrVolume* GetVolume() const { return theVolume; }
    unsigned int GetCopyNo() const { return theCopyNo; }
    const G4String& GetType() const { return theType; }
    void SetVolume(G4tgrVolume* vol) { theVolume = vol; }
    void SetType(const G4String& typ) { theType = typ; }

    virtual G4ThreeVector GetPlacement() const;

  protected:

    G4tgrVolume* theVolume = nullptr;
      // The detunit to which it belongs

    G4String theParentName = "";
      // The parent (by name, as we will allow that a child
      // is placed in the file before the parent is created)

    unsigned int theCopyNo = 0;
      // The copy number

    G4String theType = "";
      // The type (simple/replica/param)
};

#endif
