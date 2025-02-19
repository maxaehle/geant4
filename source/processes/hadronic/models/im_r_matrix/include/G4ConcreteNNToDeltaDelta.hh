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
//

#ifndef G4ConcreteNNToDeltaDelta_HH
#define G4ConcreteNNToDeltaDelta_HH

#include "globals.hh"
#include "G4VScatteringCollision.hh"
#include "G4VCrossSectionSource.hh"
#include "G4VAngularDistribution.hh"
#include "G4KineticTrackVector.hh"
#include <vector>
#include "G4XDeltaDeltaTable.hh"
#include "G4ConcreteNNTwoBodyResonance.hh"

class G4ConcreteNNToDeltaDelta : public G4ConcreteNNTwoBodyResonance
{

public:

  G4ConcreteNNToDeltaDelta(const G4ParticleDefinition* aPrimary,
		       const G4ParticleDefinition* bPriamry,
		       const G4ParticleDefinition* aSecondary,
		       const G4ParticleDefinition* bSecondary);

  virtual ~G4ConcreteNNToDeltaDelta() {};
  
  virtual G4String GetName() const
  {
    return "ConcreteNNToDeltaDelta";
  }

private:
  G4ConcreteNNToDeltaDelta(const G4ConcreteNNToDeltaDelta &);
  G4ConcreteNNToDeltaDelta & operator= (const G4ConcreteNNToDeltaDelta &);
  G4bool operator==(const G4ConcreteNNToDeltaDelta &right) const;
  G4bool operator!=(const G4ConcreteNNToDeltaDelta &right) const;

private:  

  static G4ThreadLocal G4XDeltaDeltaTable *theSigmaTable_G4MT_TLS_;

};

#endif
