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
// File name:     G4eeToHadronsMultiModel
//
// Author:        Vladimir Ivanchenko
//
// Creation date: 18.05.2005
//
// Modifications:
//

//
// Class Description: vector of e+e- -> hadrons models
//

// -------------------------------------------------------------------
//

#ifndef G4eeToHadronsMultiModel_h
#define G4eeToHadronsMultiModel_h 1

#include "G4VEmModel.hh"
#include "G4eeToHadronsModel.hh"
#include "G4ParticleChangeForGamma.hh"
#include "G4TrackStatus.hh"
#include "Randomize.hh"
#include <CLHEP/Units/SystemOfUnits.h>
#include <CLHEP/Units/PhysicalConstants.h>
#include <vector>

class G4eeCrossSections;
class G4Vee2hadrons;

class G4eeToHadronsMultiModel : public G4VEmModel
{

public:

  explicit G4eeToHadronsMultiModel(G4int ver=0, 
                                   const G4String& nam = "eeToHadrons");

  ~G4eeToHadronsMultiModel() override;

  void Initialise(const G4ParticleDefinition*, const G4DataVector&) override;

  G4double CrossSectionPerVolume(const G4Material*,
				 const G4ParticleDefinition*,
				 G4double kineticEnergy,
				 G4double cutEnergy,
				 G4double maxEnergy) override;

  G4double ComputeCrossSectionPerAtom(
                                 const G4ParticleDefinition*,
                                 G4double kineticEnergy,
                                 G4double Z, G4double A,
                                 G4double cutEnergy = 0.0,
                                 G4double maxEnergy = DBL_MAX) override;

  void SampleSecondaries(std::vector<G4DynamicParticle*>*,
			 const G4MaterialCutsCouple*,
			 const G4DynamicParticle*,
			 G4double tmin = 0.0,
			 G4double maxEnergy = DBL_MAX) override;

  void ModelDescription(std::ostream& outFile) const override;
  
  // Set the factor to artificially increase the crossSection (default 1)
  void SetCrossSecFactor(G4double fac);

  G4double ComputeCrossSectionPerElectron(const G4ParticleDefinition*,
                                          G4double kineticEnergy,
                                          G4double cutEnergy = 0.0,
                                          G4double maxEnergy = DBL_MAX);

  // hide assignment operator
  G4eeToHadronsMultiModel & operator=(const  G4eeToHadronsMultiModel &right) = delete;
  G4eeToHadronsMultiModel(const  G4eeToHadronsMultiModel&) = delete;

private:

  void AddEEModel(G4Vee2hadrons*, const G4DataVector&);

  //change incident e+ kinetic energy into CM total energy(sum of e+ and e-)  
  inline G4double LabToCM(G4double);

  G4eeCrossSections*               cross = nullptr;
  G4ParticleChangeForGamma*        fParticleChange = nullptr;

  std::vector<G4eeToHadronsModel*> models;

  std::vector<G4double>            ekinMin;
  std::vector<G4double>            ekinPeak;
  std::vector<G4double>            ekinMax;
  std::vector<G4double>            cumSum;

  G4double 			   delta;  
  G4double                         thKineticEnergy = DBL_MAX;
  G4double                         maxKineticEnergy;
  G4double                         csFactor = 1.0;

  G4int                            nModels = 0;
  G4int                            verbose;
  G4bool                           isInitialised = false;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

//change incident e+ kinetic energy into CM total energy(sum of e+ and e-)  
inline G4double G4eeToHadronsMultiModel::LabToCM(G4double kinE_lab)
{
  G4double totE_CM = 0.0;
  G4double mass = CLHEP::electron_mass_c2;
  G4double totE_lab = kinE_lab + mass;
  totE_CM = std::sqrt(2*mass*(mass+totE_lab));

  return totE_CM;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#endif
