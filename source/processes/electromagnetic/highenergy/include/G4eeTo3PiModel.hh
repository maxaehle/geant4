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
// -------------------------------------------------------------------
//
// GEANT4 Class header file
//
//
// File name:     G4eeTo3PiModel
//
// Author:        Vladimir Ivanchenko
//
// Creation date: 25.10.2003
//
// Modifications:
//

//
// Class Description:
//

// -------------------------------------------------------------------
//

#ifndef G4eeTo3PiModel_h
#define G4eeTo3PiModel_h 1

#include "G4Vee2hadrons.hh"
#include "globals.hh"
#include "G4eeCrossSections.hh"

class G4DynamicParticle;
class G4PhysicsVector;

class G4eeTo3PiModel : public G4Vee2hadrons
{

public:

  explicit G4eeTo3PiModel(G4eeCrossSections*,G4double,G4double);

  ~G4eeTo3PiModel() override;

  G4double PeakEnergy() const override;

  G4double ComputeCrossSection(G4double) const override;

  void SampleSecondaries(std::vector<G4DynamicParticle*>*,
              G4double, const G4ThreeVector&) override;

  // hide assignment operator
  G4eeTo3PiModel & operator=(const  G4eeTo3PiModel &right) = delete;
  G4eeTo3PiModel(const  G4eeTo3PiModel&) = delete;

private:

  G4double massPi;
  G4double massPi0;
  G4double massOm;
  G4double massPhi;
  G4double gmax;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#endif
