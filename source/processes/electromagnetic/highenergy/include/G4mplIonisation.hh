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
// File name:     G4mplIonisation
//
// Author:        Vladimir Ivanchenko
//
// Creation date: 25.08.2005
//
// Modifications:
//
//
// Class Description:
//
// This class manages the ionisation process for a magnetic monopole
// it inherites from G4VContinuousDiscreteProcess via G4VEnergyLossProcess.
// Magnetic charge of the monopole should be defined in the constructor of 
// the process, unless it is assumed that it is classic Dirac monopole with 
// the charge 67.5*eplus. The name of the particle should be "monopole".
//

// -------------------------------------------------------------------
//

#ifndef G4mplIonisation_h
#define G4mplIonisation_h 1

#include "G4VEnergyLossProcess.hh"
#include "globals.hh"
#include "G4VEmModel.hh"

class G4Material;
class G4VEmFluctuationModel;

class G4mplIonisation : public G4VEnergyLossProcess
{

public:

  explicit G4mplIonisation(G4double mCharge = 0.0, 
                           const G4String& name = "mplIoni");

  ~G4mplIonisation() override;

  G4bool IsApplicable(const G4ParticleDefinition& p) override;

  G4double MinPrimaryEnergy(const G4ParticleDefinition* p,
                            const G4Material*, G4double cut) final;

  // print description in html
  void ProcessDescription(std::ostream&) const override;

  // hide assignment operator
  G4mplIonisation & operator=(const G4mplIonisation &right) = delete;
  G4mplIonisation(const G4mplIonisation&) = delete;

protected:

  virtual void InitialiseEnergyLossProcess(const G4ParticleDefinition*,
                                           const G4ParticleDefinition*) override;

private:

  G4double magneticCharge;
  G4bool isInitialised = false;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#endif
